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

typedef enum
{
	NEW_POKEMON=1,
	LOCALIZED_POKEMON=2,
	GET_POKEMON=3,
	APPEARED_POKEMON=4,
	CATCH_POKEMON=5,
	CAUGHT_POKEMON=6
}tipo_mensaje;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	tipo_mensaje codigo_operacion;
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
void enviar_mensaje_appeared(t_appeared_pokemon appeared_pokemon, int socket_team);
void* serializar_paquete(t_paquete* paquete, int *size_serializado);

#endif /* CONEXION_H_ */
