#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <stdio.h>
#include <stdlib.h>
#include "message-queue.h"
#include "utils.h"

typedef enum
{
	MENSAJE=1
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct{
	int identificador;
	t_buffer* buffer;

}t_mensaje;




void recibir_mensaje(int socket_cliente,int* tamanio);

void agregar_msj_a_cola(t_mq* cola,t_buffer mensaje);

void agregar_suscriptor_cola(t_mq* cola,int suscriptor);

void enviar_mensaje_suscriptores(t_mq* cola);

#endif PROTOCOLO_H_
