#include "planificacion.h"

t_list* estimar_rafagas_entrenadores(t_list* entrenadores_a_planificar){
	return list_map(entrenadores_a_planificar, estimar_siguiente_rafaga);
}

float estimar_siguiente_rafaga(entrenador* entrenador){
	float estimacion = alpha * entrenador->accion_a_ejecutar->cpu_requerido +
			(1-alpha) * entrenador->accion_a_ejecutar->cpu_estimado_anterior;
	entrenador->accion_a_ejecutar->cpu_estimado_anterior = estimacion;

	return estimacion;

	//No se como hacer con la estimacion anterior, habia pensado en crear una variable para cada entrenador
	//tipo, en el struct entrenador, poner una variable.
	//Que guarde la rafaga anterior, no se. Si se te ocurre otra cosa mejor, avisa.
	// Igual, por ahora no cambie otra cosa que no sea en planificacion.
}

int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2){
	return estimar_siguiente_rafaga(entrenador1) <= estimar_siguiente_rafaga(entrenador2);
}

int entrenador_en_ready(entrenador* entrenador){
	return entrenador->estado == READY;
}

int entrenador_necesita_planificarse(entrenador* entrenador){
	if(entrenador_en_ready(entrenador) || entrenador->estado == BLOCK_READY){
		return 1;
	}
	else{
		return 0;
	}
}

int hay_entrenadores_para_planificar(t_list* entrenadores_a_planificar){
	return list_any_satisfy(entrenadores_a_planificar,entrenador_necesita_planificarse);
}

//ARREGLAR, devuelve pthread_t?
entrenador* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_a_planificar){
	t_list* entrenadores_en_ready = list_filter(entrenadores_a_planificar,entrenador_en_ready);
	return list_sorted(entrenadores_en_ready,tiene_menor_rafaga);
}


void planificar(t_list* entrenadores_a_planificar){
	//EN EL SWITCH TIENEN QUE IR VALORES INT
	//QUE LA PLANIFICACION SEA UN HILO QUE ARRANCA Y ESTA SIEMPRE CORRIENDO, VER DE CAMBIARLO
	switch (leer_algoritmo_planificacion()) {
	    case FIFO:
	    	fifo(lista_a_cola(entrenadores_a_planificar));
	    	break;
	    case RR:
	    	round_robin(lista_a_cola(entrenadores_a_planificar));
	    	break;
	    case SJFCD:
	    	sjf_sin_desalojo(entrenadores_a_planificar);
	    	break;
	    case SJFSD:
	    	sjf_con_desalojo(entrenadores_a_planificar);
	    	break;
	    case ALGORITMO_DESCONOCIDO:
	    	printf("El algoritmo ingresado no existe \n");
	    	break;
	}

}


//NO ESTOY TENIENDO EN CUENTA LAS ENTRADAS/SALIDAS, no se como manejarlas en codigo y como
//hacer toda esa parte

void fifo(t_queue* entrenadores_a_planificar){

	while(!queue_is_empty(entrenadores_a_planificar)){
		entrenador* entrenador_a_ejecutar = queue_pop(entrenadores_a_planificar);

		while(cpu_restante_entrenador(entrenador_a_ejecutar)){
			ejecutar(entrenador_a_ejecutar);
		}

	}

}

entrenador* regla_RR(entrenador* entrenador1, entrenador* entrenador2){
}


void round_robin(t_queue* entrenadores_a_planificar){

	int tiempo = 0;
	int quantum_consumido = 1;  //Lo seteo en 1 , porque puse <= en el IF

	//QUANTUM = 2   (Del ejemplo del config)


	while(!queue_is_empty(entrenadores_a_planificar)){
			entrenador* entrenador_a_ejecutar = queue_pop(entrenadores_a_planificar);

			while(cpu_restante_entrenador(entrenador_a_ejecutar) && quantum_consumido <= quantum){
				ejecutar(entrenador_a_ejecutar);
				tiempo ++;
				quantum_consumido ++;
			}

			if(cpu_restante_entrenador(entrenador_a_ejecutar)){
				queue_push(entrenadores_a_planificar,entrenador_a_ejecutar);
			}



	}

}

void sjf_sin_desalojo(t_list* entrenadores_a_planificar){
	t_list* estimaciones_rafagas_entrenadores = estimar_rafagas_entrenadores(entrenadores_a_planificar);

	while(hay_entrenadores_para_planificar()){
		entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada(entrenadores_a_planificar);

		while(cpu_restante_entrenador(entrenador_a_ejecutar)){
			ejecutar(entrenador_a_ejecutar);
		}
	}

}

void sjf_con_desalojo(t_list* entrenadores_a_planificar){
	t_list* estimaciones_rafagas_entrenadores = estimar_rafagas_entrenadores(entrenadores_a_planificar);

		while(hay_entrenadores_para_planificar()){
			entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada(entrenadores_a_planificar);

			ejecutar(entrenador_a_ejecutar);

		}
}

t_queue* lista_a_cola(t_list* lista) {
	t_queue* cola = queue_create();
	for(int i = 0; i< list_size(lista); i++) {
		queue_push(cola, list_get(lista, i));
	}
	return cola;
}
