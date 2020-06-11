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
#define NUMERO_MODULO 1

pthread_mutex_t lock_de_planificacion;
pthread_mutex_t lock_de_entrenador_disponible;
pthread_mutex_t mutex_entrenadores;

t_log* logger;
t_log* nuestro_log;

t_list* entrenadores;
t_dictionary* objetivo_global;
//TODO VER MAS ADELANTE EL CASO EN EL QUE LOS ENTRENADORES ESTEN OCUPADOS Y NINGUNO PUEDA IR A BUSCARLO
t_queue* pokemons_sin_entrenador;
t_list* entrenadores_ready;
t_list* intercambios;


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
	INTERCAMBIO = 6,
	EXIT = 7

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
	int id;
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
	t_list* pokemons_sobrantes;
	pokemon* pokemon_en_busqueda;
	t_queue* acciones;
} entrenador;

typedef struct
{
	entrenador* entrenador1;
	entrenador* entrenador2;
	char* pokemon1;
	char* pokemon2;
} intercambio;

//CONEXIONES
int conexion_appeared;
int conexion_localized;
int conexion_caught;
int funciona_broker;


t_list* lista_ids_localized;
pthread_mutex_t mutex_lista_ids_localized;

t_list* lista_ids_caught;
pthread_mutex_t mutex_lista_ids_caught;

typedef enum
{
	MENSAJE = 1,
	SUSCRIPCION = 2
}codigo_operacion;

typedef enum
{
	GET = 1,
	LOCALIZED = 2,
	CATCH = 3,
	CAUGHT = 4,
	NEW_ACCION = 5,
	APPEARED = 6
}codigo_accion;

typedef struct
{
	int tamanio;
	void* stream;
} t_buffer;

typedef struct
{
	int numero_de_modulo;
	codigo_operacion codigo_de_operacion;
	codigo_accion codigo_de_accion;
	t_buffer* buffer;
} t_paquete;



void iniciar_variables_globales();

void armar_entrenadores();
entrenador* armar_entrenador(char* posicion, char* pokemons, char* objetivos, int id);
void actualizar_objetivo_y_sobrante_del_entrenador_con_adquiridos(entrenador* entrenador);
void actualizar_objetivo_y_sobrante_del_entrenador(entrenador* entrenador, char* pokemon_adquirido);
int devolver_posicion_en_la_lista_del_pokemon(t_list* lista_pokemons, char* pokemon_a_buscar);
void armar_objetivo_global();

void agregar_objetivo_a_objetivo_global(char* pokemon_objetivo);
void restar_adquirido_a_objetivo_global(char* pokemon_adquirido);

posicion* armar_posicion(char* posicion_a_armar);
accion* armar_accion(void(*funcion)(void*), int cpu_requerido);



#endif /* CONSTRUCTOR_H_ */
