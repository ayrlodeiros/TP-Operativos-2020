#ifndef UTILS_H_
#define UTILS_H_

#include "config-reader.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/queue.h>
#include<readline/readline.h>

typedef struct
{
	int posicion_x;
	int posicion_y;
} posicion;

typedef struct
{
	posicion* posicion;
	int cant_maxima_pokemons;
	t_list* pokemons_adquiridos;
	t_list* pokemons_objetivo;
} entrenador;

t_log* logger;
t_log* nuestro_log;
int socket_broker;
t_list* entrenadores;

void conectar_broker(void);
void armar_entrenadores(void);
entrenador* armar_entrenador(char* posicion, char* pokemons, char * objetivos);
posicion* armar_posicion(char* posicion_a_armar);
size_t obtener_tamanio_entrenador(char* posicion, char* pokemons, char* objetivos);

#endif /* UTILS_H_ */
