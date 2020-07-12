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

void planificar();

void fifo();
void round_robin();
void sjf_sin_desalojo();
void sjf_con_desalojo();

double estimar_siguiente_rafaga(entrenador* entrenador);
void asignar_rafaga_estimada_al_entrenador(entrenador* entrenador);
int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2);
entrenador* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_con_rafagas_estimadas);

entrenador* obtener_primer_entrenador_ready();
void evaluar_y_atacar_deadlock();

void terminar_team();


#endif /* PLANIFICACION_H_ */
