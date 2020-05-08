/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	void * magic = malloc(bytes);
	int offset = 0;

	memcpy(magic + offset, &(paquete->codigo_operacion),sizeof(int));
	offset += sizeof(int);
	memcpy(magic + offset,&(paquete->buffer->size),sizeof(int));
	offset += sizeof(int);
	memcpy(magic + offset,paquete->buffer->stream, paquete->buffer->size);

	return magic;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		log_error(logger, "error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
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

//TODO
char* recibir_mensaje(int socket_cliente)
{
	log_info(logger, "Comienza a recibir el mensaje");

	t_paquete* paquete = malloc(sizeof(paquete));
	paquete->buffer = malloc(sizeof(t_buffer));


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

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
