/*
 * mensajes.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef MENSAJES_H_
#define MENSAJES_H_


typedef enum
{
	MENSAJE = 1,
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


#include<commons/log.h>
#include<commons/config.h>
#include<stdio.h>
#include<stdlib.h>

void enviar_mensaje(char* mensaje, int socket_cliente, t_log * logger);
char* recibir_mensaje(int socket_cliente, t_log * logger);



#endif /* MENSAJES_H_ */
