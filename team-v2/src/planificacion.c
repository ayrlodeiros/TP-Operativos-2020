#include "planificacion.h"


void planificar(){

	log_info(nuestro_log, "Estoy en el planificador");

	switch (leer_algoritmo_planificacion()) {
	    case FIFO:
	    	fifo();
	    	break;
	    case RR:
	    	round_robin();
	    	break;
	    case SJFSD:
	    	sjf_sin_desalojo();
	    	break;
	    case SJFCD:
	    	sjf_con_desalojo();
	    	break;
	    case ALGORITMO_DESCONOCIDO:
	    	log_error(nuestro_log, "El algoritmo ingresado no existe");
	    	break;
	}

}

void fifo(){

	while(!terminaron_todos_los_entrenadores()){
		if(list_size(entrenadores_ready) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}

		entrenador* entrenador_a_ejecutar = obtener_primer_entrenador_ready();

		while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0){
			ejecutar(entrenador_a_ejecutar);
		}

		if(entrenador_a_ejecutar->estado == BLOCK_READY) {
			//Mando señal de que hay entrenador disponible para que pueda replanificar si quedaron pokemons sin atender
			pthread_mutex_unlock(&lock_de_entrenador_disponible);
		}

		evaluar_y_atacar_deadlock();

	}

	terminar_team();
}

void round_robin(){

	int quantum_restante;

	while(!terminaron_todos_los_entrenadores()){
		if(list_size(entrenadores_ready) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}

		entrenador* entrenador_a_ejecutar = obtener_primer_entrenador_ready();

		quantum_restante = leer_quantum();

		while(quantum_restante > 0 && list_size(entrenador_a_ejecutar->acciones) > 0){

			accion* proxima_accion = list_get(entrenador_a_ejecutar->acciones, 0);

			if(proxima_accion->cpu_requerido <= entrenador_a_ejecutar->cpu_rr_anterior + quantum_restante) {
				log_info(nuestro_log, "El entrenador %d consume %d de QUANTUM", entrenador_a_ejecutar->id, (proxima_accion->cpu_requerido - entrenador_a_ejecutar->cpu_rr_anterior));

				quantum_restante -= proxima_accion->cpu_requerido;
				ejecutar(entrenador_a_ejecutar);

				//Vuelvo a setear el sobrante en 0
				entrenador_a_ejecutar->cpu_rr_anterior = 0;

				if(entrenador_a_ejecutar->estado == BLOCK_READY) {
					//Mando señal de que hay entrenador disponible para que pueda replanificar si quedaron pokemons sin atender
					pthread_mutex_unlock(&lock_de_entrenador_disponible);
				}

			} else {
				log_info(nuestro_log, "El entrenador %d consume %d de QUANTUM", entrenador_a_ejecutar->id, quantum_restante);
				entrenador_a_ejecutar->cpu_rr_anterior += quantum_restante;
				quantum_restante = 0;
			}
		}

		log_info(nuestro_log, "El entrenador %d se quedo sin QUANTUM o sin acciones para ejecutar", entrenador_a_ejecutar->id);


		if(list_size(entrenador_a_ejecutar->acciones) > 0){
			list_add(entrenadores_ready,entrenador_a_ejecutar);
		}

		evaluar_y_atacar_deadlock();
	}

	terminar_team();
}

void sjf_sin_desalojo(){
	t_list* entrenadores_con_rafagas_estimadas = list_create();

	while(!terminaron_todos_los_entrenadores()){
		log_info(nuestro_log, "Estoy en SJF SIN DESALOJO preparado para planificar");

		if(list_size(entrenadores_ready) == 0 && list_size(entrenadores_con_rafagas_estimadas) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}

		pthread_mutex_lock(&mutex_entrenadores_ready);
		while(list_size(entrenadores_ready) > 0){
			entrenador* entrenador_a_estimar = list_remove(entrenadores_ready, 0);
			asignar_rafaga_estimada_al_entrenador(entrenador_a_estimar);
			list_add(entrenadores_con_rafagas_estimadas, entrenador_a_estimar);
		}
		pthread_mutex_unlock(&mutex_entrenadores_ready);

		entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada(entrenadores_con_rafagas_estimadas);

		while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0){
			log_info(nuestro_log, "Estimado de entrenador %d es %f", entrenador_a_ejecutar->id, entrenador_a_ejecutar->cpu_estimado_restante);
			ejecutar(entrenador_a_ejecutar);
		}

		if(entrenador_a_ejecutar->estado == BLOCK_READY) {
			//Mando señal de que hay entrenador disponible para que pueda replanificar si quedaron pokemons sin atender
			pthread_mutex_unlock(&lock_de_entrenador_disponible);
		}

		evaluar_y_atacar_deadlock();
	}

	list_destroy(entrenadores_con_rafagas_estimadas);
	terminar_team();

}

void sjf_con_desalojo(){

	t_list* entrenadores_con_rafagas_estimadas = list_create();

	while(!terminaron_todos_los_entrenadores()){
		log_info(nuestro_log, "Estoy en SJF CON DESALOJO preparado para planificar");
		if(list_size(entrenadores_ready) == 0 && list_size(entrenadores_con_rafagas_estimadas) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}

		//Cada ciclo de cpu voy a evaluar si hay nuevos entrenadores en ready (mayor overhead)
		pthread_mutex_lock(&mutex_entrenadores_ready);
		while(list_size(entrenadores_ready) > 0){
			entrenador* entrenador_a_estimar = list_remove(entrenadores_ready, 0);
			asignar_rafaga_estimada_al_entrenador(entrenador_a_estimar);
			list_add(entrenadores_con_rafagas_estimadas, entrenador_a_estimar);
		}
		pthread_mutex_unlock(&mutex_entrenadores_ready);

		entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada(entrenadores_con_rafagas_estimadas);

		accion* proxima_accion = list_get(entrenador_a_ejecutar->acciones, 0);

		log_info(nuestro_log, "Estimado restante de entrenador %d es %f", entrenador_a_ejecutar->id, entrenador_a_ejecutar->cpu_estimado_restante);
		log_info(nuestro_log, "Entrenador %d consume 1 de CPU", entrenador_a_ejecutar->id);
		if(proxima_accion->cpu_requerido == entrenador_a_ejecutar->cpu_sjf_anterior) {
			ejecutar(entrenador_a_ejecutar);
			//Vuelvo a setear el cpu acumulado anterior en 1
			entrenador_a_ejecutar->cpu_sjf_anterior = 1;

			if(entrenador_a_ejecutar->estado == BLOCK_READY) {
				//Mando señal de que hay entrenador disponible para que pueda replanificar si quedaron pokemons sin atender
				pthread_mutex_unlock(&lock_de_entrenador_disponible);
			}
			//Cuando realmente se ejecuta la accion es cuando hay que evaluar el deadlock
			evaluar_y_atacar_deadlock();
		} else {
			//Voy sumando 1 de CPU
			entrenador_a_ejecutar->cpu_sjf_anterior += 1;
		}

		entrenador_a_ejecutar->cpu_estimado_restante -= 1;

		if(list_size(entrenador_a_ejecutar->acciones) > 0) {
			//Vuelvo a agregar al entrenador para que siga siendo evaluado porque sigue teniendo acciones que ejecutar
			list_add(entrenadores_con_rafagas_estimadas, entrenador_a_ejecutar);
		}
	}

	list_destroy(entrenadores_con_rafagas_estimadas);
	terminar_team();
}

double estimar_siguiente_rafaga(entrenador* entrenador){
	double config_alpha = leer_alpha();
	double estimacion = config_alpha * cpu_restante_entrenador(entrenador) +
			(1-config_alpha) * entrenador->cpu_estimado_anterior;

	return estimacion;
}

void asignar_rafaga_estimada_al_entrenador(entrenador* entrenador){
	entrenador->cpu_estimado_anterior = estimar_siguiente_rafaga(entrenador);
	entrenador->cpu_estimado_restante = entrenador->cpu_estimado_anterior;
}

int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2){
	return entrenador1->cpu_estimado_restante <= entrenador2->cpu_estimado_restante;
}

entrenador* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_con_rafagas_estimadas){
	list_sort(entrenadores_con_rafagas_estimadas,tiene_menor_rafaga);
	return list_remove(entrenadores_con_rafagas_estimadas,0);
}




entrenador* obtener_primer_entrenador_ready() {
	pthread_mutex_lock(&mutex_entrenadores_ready);
	entrenador* entrenador_ready = list_remove(entrenadores_ready, 0);
	pthread_mutex_unlock(&mutex_entrenadores_ready);
	return entrenador_ready;
}

void evaluar_y_atacar_deadlock() {
	t_list* entrenadores_en_deadlock = entrenadores_con_block_deadlock();
	if(list_size(entrenadores_en_deadlock) >= 2 ){
		entrenador* ultimo_entrenador_en_deadlock = list_get(entrenadores_en_deadlock, (list_size(entrenadores_en_deadlock) - 1));

		planear_intercambio(ultimo_entrenador_en_deadlock);
	}
	list_destroy(entrenadores_en_deadlock);
}

