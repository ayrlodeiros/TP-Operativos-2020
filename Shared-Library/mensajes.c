/*
 * mensajes.c
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#include "mensajes.h"


void enviar_mensaje(char* mensaje, int socket_cliente,t_log * logger)
{
	log_info(logger, "Comienza a enviar el mensaje");

	char* string = mensaje;

	int tamanio_mensaje = strlen(string) + 1;
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = tamanio_mensaje;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset,&string,tamanio_mensaje);
	buffer->stream = stream;

	t_paquete* paquete = malloc(sizeof(paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = buffer;

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);


	if(send(socket_cliente,a_enviar, bytes ,0) > 0){
		log_info(logger, "Se mando el mensaje correctamente");
	}
	else{
		log_error(logger, "No se mando una mierda");
	}

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

char* recibir_mensaje(int socket_cliente, t_log * logger)
{
	log_info(logger, "Comienza a recibir el mensaje");

	t_paquete* paquete = malloc(sizeof(paquete));


	if(recv(socket_cliente,&(paquete->codigo_operacion),sizeof(int),0) > 0){
		log_info(logger, "Se recibio el codigo de operacion correctamente");
	}

	if(recv(socket_cliente,&(paquete->buffer->size),sizeof(int),0) > 0){
			log_info(logger, "Se recibio el tamanio del buffer correctamente");
		}

	if(recv(socket_cliente,&(paquete->buffer->stream),paquete->buffer->size,0) > 0){
				log_info(logger, "Se recibio el stream correctamente");
			}

	char* string = paquete->buffer->stream;

	free(paquete);

	return string;
}

