#include "planificacion.h"

<<<<<<< HEAD
/*t_list* estimar_rafagas_entrenadores(){
	return list_map(entrenadores_ready, estimar_siguiente_rafaga);
}

float estimar_siguiente_rafaga(entrenador* entrenador){
	float alpha = 0.5;
	float estimacion = alpha * entrenador->cpu_disponible +
			(1-alpha) * entrenador->cpu_estimado_anterior;

	entrenador->cpu_estimado_anterior = estimacion;

	printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
	printf("\n ESTIMACION DE RAFAGA ENTRENADOR: %d",estimacion);

	return estimacion;
=======

void planificar(){

	log_info(nuestro_log, "Estoy en el planificador");
	cantidad_de_cambios_de_contexto = 0;

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

	int id_entrenador_anterior = -1;
>>>>>>> develop

	for(int j = 0; j< list_size(entrenadores);j++){
		evaluar_y_atacar_deadlock();
	}

	while(!terminaron_todos_los_entrenadores()){
		if(list_size(entrenadores_ready) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}

		entrenador* entrenador_a_ejecutar = obtener_primer_entrenador_ready();

		//Verifico si el entrenador anterior es distinto al de ahora, si es asi, quiere decir que hubo cambio de contexto
		evaluar_cambio_de_contexto(id_entrenador_anterior, entrenador_a_ejecutar->id);
		id_entrenador_anterior = entrenador_a_ejecutar->id;

		while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0){
			ejecutar(entrenador_a_ejecutar);
		}

		evaluar_si_entrenador_termino(entrenador_a_ejecutar);

		evaluar_y_atacar_deadlock();

	}

	terminar_team();
}

<<<<<<< HEAD
int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2){
	return estimar_siguiente_rafaga(entrenador1) <= estimar_siguiente_rafaga(entrenador2);
}

//ARREGLAR, devuelve pthread_t?
entrenador* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_a_planificar){
	list_sort(entrenadores_ready,tiene_menor_rafaga);
	return list_remove(entrenadores_ready,0);
}
*/

void planificar(){
	//EN EL SWITCH TIENEN QUE IR VALORES INT
	//QUE LA PLANIFICACION SEA UN HILO QUE ARRANCA Y ESTA SIEMPRE CORRIENDO, VER DE CAMBIARLO
	switch (leer_algoritmo_planificacion()) {
	    case FIFO:
	    	fifo();
	    	break;
	    case RR:
	    	round_robin();
	    	break;
	    case SJFSD:
	    	//sjf_sin_desalojo();
	    	break;
	    case SJFCD:
	    	//sjf_con_desalojo();
	    	break;
	    case ALGORITMO_DESCONOCIDO:
	    	log_error(nuestro_log, "El algoritmo ingresado no existe");
	    	break;
=======
void round_robin(){

	int quantum_restante;
	int id_entrenador_anterior = -1;

	for(int j = 0; j< list_size(entrenadores);j++){
		evaluar_y_atacar_deadlock();
	}

	while(!terminaron_todos_los_entrenadores()){
		if(list_size(entrenadores_ready) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}

		entrenador* entrenador_a_ejecutar = obtener_primer_entrenador_ready();

		//Verifico si el entrenador anterior es distinto al de ahora, si es asi, quiere decir que hubo cambio de contexto
		evaluar_cambio_de_contexto(id_entrenador_anterior, entrenador_a_ejecutar->id);
		id_entrenador_anterior = entrenador_a_ejecutar->id;

		quantum_restante = leer_quantum();

		while(quantum_restante > 0 && list_size(entrenador_a_ejecutar->acciones) > 0){

			accion* proxima_accion = list_get(entrenador_a_ejecutar->acciones, 0);

			if(proxima_accion->cpu_requerido <= entrenador_a_ejecutar->cpu_rr_anterior + quantum_restante) {
				log_info(nuestro_log, "El entrenador %d consume %d de QUANTUM", entrenador_a_ejecutar->id, (proxima_accion->cpu_requerido - entrenador_a_ejecutar->cpu_rr_anterior));

				quantum_restante -= proxima_accion->cpu_requerido;
				ejecutar(entrenador_a_ejecutar);

				//Vuelvo a setear el sobrante en 0
				entrenador_a_ejecutar->cpu_rr_anterior = 0;

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

		evaluar_si_entrenador_termino(entrenador_a_ejecutar);

		evaluar_y_atacar_deadlock();
	}

	terminar_team();
}

void sjf_sin_desalojo(){
	t_list* entrenadores_con_rafagas_estimadas = list_create();
	int id_entrenador_anterior = -1;

	for(int j = 0; j< list_size(entrenadores);j++){
		evaluar_y_atacar_deadlock();
	}

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

		//Verifico si el entrenador anterior es distinto al de ahora, si es asi, quiere decir que hubo cambio de contexto
		evaluar_cambio_de_contexto(id_entrenador_anterior, entrenador_a_ejecutar->id);
		id_entrenador_anterior = entrenador_a_ejecutar->id;

		while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0){
			log_info(nuestro_log, "Estimado de entrenador %d es %f", entrenador_a_ejecutar->id, entrenador_a_ejecutar->cpu_estimado_restante);
			ejecutar(entrenador_a_ejecutar);
		}

		evaluar_si_entrenador_termino(entrenador_a_ejecutar);
		evaluar_y_atacar_deadlock();
	}

	list_destroy(entrenadores_con_rafagas_estimadas);
	terminar_team();

}

void sjf_con_desalojo(){

	t_list* entrenadores_con_rafagas_estimadas = list_create();
	int id_entrenador_anterior = -1;

	for(int j = 0; j< list_size(entrenadores);j++){
		evaluar_y_atacar_deadlock();
	}

	while(!terminaron_todos_los_entrenadores()){
		log_info(nuestro_log, "Estoy en SJF CON DESALOJO preparado para planificar");
		if(list_size(entrenadores_ready) == 0 && list_size(entrenadores_con_rafagas_estimadas) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}

		if(list_size(entrenadores_ready) != 0 || list_size(entrenadores_con_rafagas_estimadas) != 0) {

			//Cada ciclo de cpu voy a evaluar si hay nuevos entrenadores en ready (mayor overhead)
			pthread_mutex_lock(&mutex_entrenadores_ready);
			while(list_size(entrenadores_ready) > 0){
				entrenador* entrenador_a_estimar = list_remove(entrenadores_ready, 0);
				asignar_rafaga_estimada_al_entrenador(entrenador_a_estimar);
				if(list_is_empty(entrenadores_con_rafagas_estimadas) || !lista_contiene_a_entrenador(entrenadores_con_rafagas_estimadas, entrenador_a_estimar)) {
					list_add(entrenadores_con_rafagas_estimadas, entrenador_a_estimar);
				}
			}
			pthread_mutex_unlock(&mutex_entrenadores_ready);

			entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada(entrenadores_con_rafagas_estimadas);

			//Verifico si el entrenador anterior es distinto al de ahora, si es asi, quiere decir que hubo cambio de contexto
			evaluar_cambio_de_contexto(id_entrenador_anterior, entrenador_a_ejecutar->id);
			id_entrenador_anterior = entrenador_a_ejecutar->id;

			accion* proxima_accion = list_get(entrenador_a_ejecutar->acciones, 0);

			log_info(nuestro_log, "Estimado restante de entrenador %d es %f", entrenador_a_ejecutar->id, entrenador_a_ejecutar->cpu_estimado_restante);
			log_info(nuestro_log, "Entrenador %d consume 1 de CPU", entrenador_a_ejecutar->id);
			if(proxima_accion->cpu_requerido == entrenador_a_ejecutar->cpu_sjf_anterior) {
				ejecutar(entrenador_a_ejecutar);
				//Vuelvo a setear el cpu acumulado anterior en 1
				entrenador_a_ejecutar->cpu_sjf_anterior = 1;

				evaluar_si_entrenador_termino(entrenador_a_ejecutar);
				//Cuando realmente se ejecuta la accion es cuando hay que evaluar el deadlock
				evaluar_y_atacar_deadlock();

			} else {
				//Voy sumando 1 de CPU
				entrenador_a_ejecutar->cpu_sjf_anterior += 1;
			}

			if(entrenador_a_ejecutar->estado != EXIT){
				entrenador_a_ejecutar->cpu_estimado_restante -= 1;

				if(list_size(entrenador_a_ejecutar->acciones) > 0) {
					//Vuelvo a agregar al entrenador para que siga siendo evaluado porque sigue teniendo acciones que ejecutar
					list_add(entrenadores_con_rafagas_estimadas, entrenador_a_ejecutar);
				}
			}

		}
>>>>>>> develop
	}

	list_destroy(entrenadores_con_rafagas_estimadas);
	terminar_team();
}

int lista_contiene_a_entrenador(t_list* lista, entrenador* entrenador_a_buscar) {
	for(int i = 0; i < list_size(lista); i++) {
		entrenador* entrenador_en_lista = list_get(lista, i);
		if(entrenador_en_lista->id == entrenador_a_buscar->id) {
			return 1;
		}
	}
	return 0;
}

<<<<<<< HEAD
//NO ESTOY TENIENDO EN CUENTA LAS ENTRADAS/SALIDAS, no se como manejarlas en codigo y como
//hacer toda esa parte

void fifo(){

	while(1){
		log_info(nuestro_log, "Estoy en FIFO preparado para planificar");
		if(list_size(entrenadores_ready) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}
		entrenador* entrenador_a_ejecutar = list_remove(entrenadores_ready, 0);

		while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0){
			ejecutar(entrenador_a_ejecutar);
		}
		entrenador_disponible(entrenador_a_ejecutar);
	}
}

void round_robin(){

	int tiempo = 0;
	int quantum_consumido = 1;  //Lo seteo en 1 , porque puse <= en el IF

	//QUANTUM = 2   (Del ejemplo del config)

	printf("\n QUANTUM TOTAL : %d", quantum);

	while(1){
			entrenador* entrenador_a_ejecutar = queue_pop(entrenadores_ready);

			printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
			//printf("\n CPU REQUERIDO ENTRENADOR : %d", entrenador_a_ejecutar->accion_a_ejecutar->cpu_requerido);
			printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador_a_ejecutar->posicion->posicion_x, entrenador_a_ejecutar->posicion->posicion_y);

			for(int j = 0; j<list_size(entrenador_a_ejecutar->pokemons_adquiridos); j++){
				printf("\nPOKEMONS ENTRENADOR : %s", list_get(entrenador_a_ejecutar->pokemons_adquiridos, j));
			}

			while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0 && quantum_consumido <= quantum){
				ejecutar(entrenador_a_ejecutar);
				tiempo ++;
				quantum_consumido ++;
				printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
				printf("\n CPU DISPONIBLE ENTRENADOR : %d", entrenador_a_ejecutar->cpu_disponible);
				printf("\n QUANTUM CONSUMIDO : %d", quantum_consumido);
				printf("\n CPU TIEMPO : %d", tiempo);
			}

			if(cpu_restante_entrenador(entrenador_a_ejecutar)){
				queue_push(entrenadores_ready,entrenador_a_ejecutar);
			}
			else{
				entrenador_disponible(entrenador_a_ejecutar);
			}



	}
=======
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


>>>>>>> develop


entrenador* obtener_primer_entrenador_ready() {
	pthread_mutex_lock(&mutex_entrenadores_ready);
	entrenador* entrenador_ready = list_remove(entrenadores_ready, 0);
	pthread_mutex_unlock(&mutex_entrenadores_ready);
	return entrenador_ready;
}

<<<<<<< HEAD
/*void sjf_sin_desalojo(){

	if(list_size(entrenadores_ready) == 0) {
		pthread_mutex_lock(&lock_de_planificacion);
	}

	t_list* estimaciones_rafagas_entrenadores = list_create();
	estimaciones_rafagas_entrenadores = estimar_rafagas_entrenadores();

	while(1){
		entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada();

		printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
		printf("\n CPU DISPONIBLE ENTRENADOR : %d", entrenador_a_ejecutar->cpu_disponible);
		printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador_a_ejecutar->posicion->posicion_x, entrenador_a_ejecutar->posicion->posicion_y);

		for(int j = 0; j<list_size(entrenador_a_ejecutar->pokemons_adquiridos); j++){
			printf("\nPOKEMONS ENTRENADOR : %s", list_get(entrenador_a_ejecutar->pokemons_adquiridos, j));
		}

		while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0){
			ejecutar(entrenador_a_ejecutar);
		}
		entrenador_disponible(entrenador_a_ejecutar);
	}
=======
void evaluar_cambio_de_contexto(int id_entrenador_anterior, int id_entrenador_actual) {
	if(id_entrenador_anterior != id_entrenador_actual) {
		cantidad_de_cambios_de_contexto++;
	}
}

void evaluar_si_entrenador_termino(entrenador* entrenador_a_evaluar) {
	if(entrenador_a_evaluar->estado == BLOCK_READY) {
		//Mando señal de que hay entrenador disponible para que pueda replanificar si quedaron pokemons sin atender
		pthread_mutex_unlock(&lock_de_entrenador_disponible);
	}
}

void evaluar_y_atacar_deadlock() {
	t_list* entrenadores_en_deadlock = entrenadores_con_block_deadlock();
	if(list_size(entrenadores_en_deadlock) >= 2 ){
		entrenador* ultimo_entrenador_en_deadlock = list_get(entrenadores_en_deadlock, (list_size(entrenadores_en_deadlock) - 1));
>>>>>>> develop

		planear_intercambio(ultimo_entrenador_en_deadlock);
	}
	list_destroy(entrenadores_en_deadlock);
}


<<<<<<< HEAD

void sjf_con_desalojo(){
	t_list* estimaciones_rafagas_entrenadores = estimar_rafagas_entrenadores();

	while(1){
		entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada();

		printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
		printf("\n CPU DISPONIBLE ENTRENADOR : %d", entrenador_a_ejecutar->cpu_disponible);
		printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador_a_ejecutar->posicion->posicion_x, entrenador_a_ejecutar->posicion->posicion_y);

		for(int j = 0; j<list_size(entrenador_a_ejecutar->pokemons_adquiridos); j++){
			printf("\nPOKEMONS ENTRENADOR : %s", list_get(entrenador_a_ejecutar->pokemons_adquiridos, j));
		}

		ejecutar(entrenador_a_ejecutar);

		if(cpu_restante_entrenador(entrenador_a_ejecutar) == 0){
			entrenador_disponible(entrenador_a_ejecutar);
		}

	}
}*/
=======
void terminar_team() {
	mostrar_metricas(nuestro_log);
	mostrar_metricas(logger);
	log_info(nuestro_log, "TERMINO EL PROCESO TEAM");
	destruir_config();
	log_destroy(logger);
	log_destroy(nuestro_log);
}

>>>>>>> develop
