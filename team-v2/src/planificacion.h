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

<<<<<<< HEAD
t_list* estimar_rafagas_entrenadores();
float estimar_siguiente_rafaga(entrenador* entrenador);
int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2);
entrenador* entrenador_con_menor_rafaga_estimada();

void planificar();
void fifo();
void round_robin();

/*
void sjf_sin_desalojo();
void sjf_con_desalojo();*/
=======
void planificar();

void fifo();
void round_robin();
void sjf_sin_desalojo();
void sjf_con_desalojo();

int lista_contiene_a_entrenador(t_list* lista, entrenador* entrenador_a_buscar);
double estimar_siguiente_rafaga(entrenador* entrenador);
void asignar_rafaga_estimada_al_entrenador(entrenador* entrenador);
int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2);
entrenador* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_con_rafagas_estimadas);

entrenador* obtener_primer_entrenador_ready();
void evaluar_cambio_de_contexto(int id_entrenador_anterior, int id_entrenador_actual);
void evaluar_si_entrenador_termino(entrenador* entrenador_a_evaluar);
void evaluar_y_atacar_deadlock();

void terminar_team();
>>>>>>> develop


#endif /* PLANIFICACION_H_ */
