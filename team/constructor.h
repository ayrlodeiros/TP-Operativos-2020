/*
 * constructor.h
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */

#ifndef CONSTRUCTOR_H_
#define CONSTRUCTOR_H_

#include<pthread.h>
#include<stdlib.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<commons/collections/dictionary.h>

t_list* entrenadores;
t_dictionary* objetivo_global;
t_queue* pokemons_sueltos;
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
	BLOCK = 3,
	EXIT = 4

}estado_entrenador;

typedef struct
{
	pthread_t* hilo;
	estado_entrenador estado;
	int cpu_usado;
	int cpu_disponible;
	posicion* posicion;
	int cant_maxima_pokemons;
	t_list* pokemons_adquiridos;
	t_list* pokemons_objetivo;
} entrenador;

typedef struct
{
	char* nombre;
	posicion* posicion;
} pokemon;

void iniciar_variables_globales();

void armar_entrenadores();
entrenador* armar_entrenador(char* posicion, char* pokemons, char* objetivos);
void armar_objetivo_global();

void agregar_objetivo_a_objetivo_global(char* pokemon_objetivo);
void restar_adquirido_a_objetivo_global(char* pokemon_adquirido);

void armar_pokemon(char* nombre, int posicion_x, int posicion_y);

posicion* armar_posicion(char* posicion_a_armar);



#endif /* CONSTRUCTOR_H_ */
