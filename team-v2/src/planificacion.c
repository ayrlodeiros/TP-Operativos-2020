#include "planificacion.h"

/*t_list* estimar_rafagas_entrenadores(t_list* entrenadores_a_planificar){
	return list_map(entrenadores_a_planificar, estimar_siguiente_rafaga);
}

float estimar_siguiente_rafaga(entrenador* entrenador){
	float alpha = 0.5;
	float estimacion = alpha * entrenador->accion_a_ejecutar->cpu_requerido +
			(1-alpha) * entrenador->accion_a_ejecutar->cpu_estimado_anterior;
	entrenador->accion_a_ejecutar->cpu_estimado_anterior = estimacion;

	printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
	printf("\n ESTIMACION DE RAFAGA ENTRENADOR: %d",estimacion);

	return estimacion;

	//No se como hacer con la estimacion anterior, habia pensado en crear una variable para cada entrenador
	//tipo, en el struct entrenador, poner una variable.
	//Que guarde la rafaga anterior, no se. Si se te ocurre otra cosa mejor, avisa.
	// Igual, por ahora no cambie otra cosa que no sea en planificacion.
}

int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2){
	return estimar_siguiente_rafaga(entrenador1) <= estimar_siguiente_rafaga(entrenador2);
}

//ARREGLAR, devuelve pthread_t?
entrenador* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_a_planificar){
	t_list* entrenadores_en_ready = list_filter(entrenadores_a_planificar,entrenador_en_ready);
	list_sort(entrenadores_en_ready,tiene_menor_rafaga);
	return list_get(entrenadores_en_ready,0);
}*/


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
	}

}


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



	}

}
/*
void sjf_sin_desalojo(){
	t_list* estimaciones_rafagas_entrenadores = estimar_rafagas_entrenadores(entrenadores_ready);

	while(1){
		entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada(entrenadores_ready);

		printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
		printf("\n CPU DISPONIBLE ENTRENADOR : %d", entrenador_a_ejecutar->cpu_disponible);
		printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador_a_ejecutar->posicion->posicion_x, entrenador_a_ejecutar->posicion->posicion_y);

		for(int j = 0; j<list_size(entrenador_a_ejecutar->pokemons_adquiridos); j++){
			printf("\nPOKEMONS ENTRENADOR : %s", list_get(entrenador_a_ejecutar->pokemons_adquiridos, j));
		}

		while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0){
			ejecutar(entrenador_a_ejecutar);
		}
	}

}

void sjf_con_desalojo(){
	t_list* estimaciones_rafagas_entrenadores = estimar_rafagas_entrenadores(entrenadores_ready);

	while(1){
		entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada(entrenadores_ready);

		printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
		printf("\n CPU DISPONIBLE ENTRENADOR : %d", entrenador_a_ejecutar->cpu_disponible);
		printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador_a_ejecutar->posicion->posicion_x, entrenador_a_ejecutar->posicion->posicion_y);

		for(int j = 0; j<list_size(entrenador_a_ejecutar->pokemons_adquiridos); j++){
			printf("\nPOKEMONS ENTRENADOR : %s", list_get(entrenador_a_ejecutar->pokemons_adquiridos, j));
		}

		ejecutar(entrenador_a_ejecutar);

	}
}*/
