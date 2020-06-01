/*
 * constructor.h
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */

#ifndef CONSTRUCTOR_H_
#define CONSTRUCTOR_H_

#include"config-reader.h"
#include<math.h>
#include<netdb.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/socket.h>
#include<readline/readline.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<commons/collections/dictionary.h>

static pthread_mutex_t lock_de_planificacion = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock_de_entrenador_disponible = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_entrenadores;

t_log* logger;
t_log* nuestro_log;
int funciona_broker;

t_list* entrenadores;
t_dictionary* objetivo_global;
//TODO VER MAS ADELANTE EL CASO EN EL QUE LOS ENTRENADORES ESTEN OCUPADOS Y NINGUNO PUEDA IR A BUSCARLO
t_queue* pokemons_sin_entrenador;
t_list* entrenadores_ready;

typedef struct
{
	int posicion_x;
	int posicion_y;
} posicion;

typedef enum
{
	NEW = 0,
	READY = 1,
	EXEC = 2,
	BLOCK_READY = 3,
	BLOCK_CATCHING = 4,
	BLOCK_DEADLOCK = 5,
	EXIT = 6

}estado_entrenador;

typedef struct{
	void(*funcion)(void*);
	int cpu_requerido;
}accion;

typedef struct
{
	char* nombre;
	posicion* posicion;
} pokemon;

typedef struct
{
	pthread_t* hilo;
	estado_entrenador estado;
	int cpu_usado;
	int cpu_disponible;
	double cpu_estimado_anterior;
	double cpu_estimado_restante;
	posicion* posicion;
	int cant_maxima_pokemons;
	t_list* pokemons_adquiridos;
	t_list* pokemons_objetivo;
	pokemon* pokemon_en_busqueda;
	t_queue* acciones;
} entrenador;



void iniciar_variables_globales();

void armar_entrenadores();
entrenador* armar_entrenador(char* posicion, char* pokemons, char* objetivos);
void armar_objetivo_global();

void agregar_objetivo_a_objetivo_global(char* pokemon_objetivo);
void restar_adquirido_a_objetivo_global(char* pokemon_adquirido);

posicion* armar_posicion(char* posicion_a_armar);
accion* armar_accion(void(*funcion)(void*), int cpu_requerido);



#endif /* CONSTRUCTOR_H_ */
