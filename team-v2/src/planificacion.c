#include "planificacion.h"

t_list* estimar_rafagas_entrenadores(t_list* entrenadores_a_planificar){
	//return list_map(entrenadores_a_planificar,estimar_siguiente_rafaga())
}

float estimar_siguiente_rafaga(float estimacion_anterior, int cpu_anterior_real){
	//return alpha * cpu_anterior_real + (1-alpha) * estimacion_anterior;

	//No se como hacer con la estimacion anterior, habia pensado en crear una variable para cada entrenador
	//tipo, en el struct entrenador, poner una variable.
	//Que guarde la rafaga anterior, no se. Si se te ocurre otra cosa mejor, avisa.
	// Igual, por ahora no cambie otra cosa que no sea en planificacion.
}

int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2){
	//return estimar_siguiente_rafaga(,entrenador1->cpu_usado)
}

pthread_t* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_a_planificar){

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


void fifo(t_list* entrenadores_a_planificar){

}

void round_robin(t_list* entrenadores_a_planificar){

}

void sjf_sin_desalojo(t_list* entrenadores_a_planificar){

}

void sjf_con_desalojo(t_list* entrenadores_a_planificar){

}
