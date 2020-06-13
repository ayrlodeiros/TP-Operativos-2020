/*
 * conexion.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef CONEXION_H_
#define CONEXION_H_

#include<sys/socket.h>
#include<netdb.h>
#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/string.h>
#include "message-queue.h"
#include "gameboy-v2.h"

typedef enum
{
	GET_POKEMON=1,
	LOCALIZED_POKEMON=2,
	CATCH_POKEMON=3,
	CAUGHT_POKEMON=4,
	NEW_POKEMON=5,
	APPEARED_POKEMON=6
}tipo_mensaje;

typedef enum
{
	MENSAJE=1,
	SUSCRIPCION=2
}codigo_operacion;

typedef enum
{
	TEAM = 1,
	GAMEBOY = 2,
	GAMECARD = 3,
	BROKER = 4
}tipo_modulo;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	tipo_modulo modulo;
	codigo_operacion cod_op;
	tipo_mensaje mensaje;
	t_buffer* buffer;
} t_paquete;

typedef struct{
	uint32_t largo_nombre_pokemon;
	char* nombre_pokemon;
	uint32_t posicionX;
	uint32_t posicionY;
}t_appeared_pokemon;

typedef struct{
	uint32_t largo_nombre_pokemon;
	char* nombre_pokemon;
}t_get_pokemon;

typedef struct{
	uint32_t largo_nombre_pokemon;
	char* nombre_pokemon;
	uint32_t posicionX;
	uint32_t posicionY;
	uint32_t cantidad_pokemon;
}t_new_pokemon;

typedef struct{
	uint32_t atrapado;
}t_caught_pokemon;

typedef struct{
	uint32_t largo_nombre_pokemon;
	char* nombre_pokemon;
	uint32_t posicionX;
	uint32_t posicionY;
}t_catch_pokemon;

int crear_conexion_del_cliente(char *ip, char* puerto, t_log* logger);
void liberar_conexion(int socket);
void* serializar_paquete(t_paquete* paquete, int size_serializado);
void enviar_mensaje_appeared(t_appeared_pokemon appeared_pokemon, int socket_team,int puerto, int id_mensaje_correlativo);
void enviar_mensaje_get(t_get_pokemon get_pokemon, int socket_broker,int puerto, int id_mensaje);
void enviar_mensaje_new(t_new_pokemon new_pokemon, int socket_broker,int puerto, int id_mensaje);
void enviar_mensaje_caught(t_caught_pokemon caught_pokemon, int socket_broker,int id_mensaje_correlativo);
void enviar_mensaje_catch(t_catch_pokemon catch_pokemon, int socket_broker,int puerto, int id_mensaje);
void suscribirse_a_cola(t_mq cola_de_mensajes, int tiempo, int socket_broker);

#endif /* CONEXION_H_ */
