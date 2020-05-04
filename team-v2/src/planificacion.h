/*
 * planificacion.h
 *
 *  Created on: 3 may. 2020
 *      Author: utnso
 */

#include "utils.h"

#ifndef PLANIFICACION_H_
#define PLANIFICACION_H_

float alpha = 0.5;

t_list* estimar_rafagas_entrenadores(t_list* entrenadores_a_planificar);
float estimar_siguiente_rafaga(float estimacion_anterior, int cpu_anterior_real);
int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2);
pthread_t* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_a_planificar);
void planificar(t_list* entrenadores_a_planificar);
void fifo(t_list* entrenadores_a_planificar);
void round_robin(t_list* entrenadores_a_planificar);
void sjf_sin_desalojo(t_list* entrenadores_a_planificar);
void sjf_con_desalojo(t_list* entrenadores_a_planificar);



#endif /* PLANIFICACION_H_ */
