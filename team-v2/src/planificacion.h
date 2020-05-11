/*
 * planificacion.h
 *
 *  Created on: 3 may. 2020
 *      Author: utnso
 */


//VER SI ESTO ESTA BIEN
#include "utils.h"

#ifndef PLANIFICACION_H_
#define PLANIFICACION_H_

/*t_list* estimar_rafagas_entrenadores(t_list* entrenadores_a_planificar);
float estimar_siguiente_rafaga(entrenador* entrenador);
int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2);
entrenador* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_a_planificar);*/

void planificar();
void fifo();
void round_robin();
/*
void sjf_sin_desalojo();
void sjf_con_desalojo();*/


#endif /* PLANIFICACION_H_ */
