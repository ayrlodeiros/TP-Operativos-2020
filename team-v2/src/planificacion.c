#include "planificacion.h"

t_list* estimar_rafagas_entrenadores(t_list* entrenadores_a_planificar){
	return list_map(entrenadores_a_planificar,estimar_siguiente_rafaga());
}

float estimar_siguiente_rafaga(entrenador* entrenador){
	float estimacion = alpha * entrenador->accion_a_ejecutar->cpu_requerido + (1-alpha) * entrenador->accion_a_ejecutar->cpu_estimado_anterior;
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

pthread_t* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_a_planificar){
	return list_sorted(entrenadores_a_planificar,tiene_menor_rafaga());
}


void planificar(t_list* entrenadores_a_planificar){

	switch (algoritmo_planificacion)
	​{
	    case "FIFO":
	    	fifo(entrenadores_a_planificar);
	    	break;

	    case "RR":
	    	round_robin(entrenadores_a_planificar);
	    	break;

	    case "SJF-CD":
	    	sjf_sin_desalojo(entrenadores_a_planificar);
	    	break;
	    case "SJF-SD":
	    	sjf_con_desalojo(entrenadores_a_planificar);
	    	break;

	    default:
	      printf("El algoritmo ingresado no existe \n");
	}

}


//NO ESTOY TENIENDO EN CUENTA LAS ENTRADAS/SALIDAS, no se como manejarlas en codigo y como
//hacer toda esa parte

void fifo(t_queue* entrenadores_a_planificar){



	while(not(queue_is_empty(entrenadores_a_planificar))){
		entrenador* entrenador_a_ejecutar = queue_pop(entrenadores_a_planificar);

		while(cpu_restante_entrenador(entrenador_a_ejecutar)){
			ejecutar(entrenador_a_ejecutar);
		}

	}

}

entrenador* regla_RR(t_queue* entrenadores_a_planificar , int tiempo){

}

void round_robin(t_queue* entrenadores_a_planificar){

	int tiempo;
	int quantum_consumido = 1;  //Lo seteo en 1 , porque puse <= en el IF

	//QUANTUM = 2   (Del ejemplo del config)


	while(not(queue_is_empty(entrenadores_a_planificar))){
			entrenador* entrenador_a_ejecutar = queue_pop(entrenadores_a_planificar);

			while(cpu_restante_entrenador(entrenador_a_ejecutar) && quantum_consumido <= quantum){
				ejecutar(entrenador_a_ejecutar);
				tiempo ++;
				quantum_consumido ++;
			}


	}

}

void sjf_sin_desalojo(t_list* entrenadores_a_planificar){


}

void sjf_con_desalojo(t_list* entrenadores_a_planificar){

}
