#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <stdio.h>
#include <stdlib.h>
#include "message-queue.h"
#include "utils.h"

typedef enum
{
	MENSAJE=1,
	SUSCRIPCION=2
}op_code; /** Para identificar cuando se recibe una conexion que tipo de operacion se va a realizar*/

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct{
	int id;
	int id_cor;
	modulo_code modulo;
	t_list* suscriptores; /** Todavia no estoy seguro si hago dos listas de suscriptores (a los que ya se envio y los que falta recibir confirmacion),
						o una sola lista de una estrucutura que tenga el sucriptor y su estado */
	t_buffer* buffer;

}t_mensaje; /** Es la estructura con la que se va a guardar los mensajes en la memoria interna del broker, contiene mas informacion */

typedef struct{
	int id;
	int id_cor;
	t_buffer* buffer;
}t_paquete; /**  Es la estructura con la que se va a enviar los mensajes a los modulos */

void agregar_suscriptor_cola(t_mq* cola,int suscriptor);

/**Tambien se podria separar en dos funciones */
void recibir_y_guardar_mensaje(int socket_cliente,t_mq* queue);

t_mensaje* crear_mensaje(void* buffer,int tamanio);

void agregar_msj_cola(t_mq* cola,t_mensaje* mensaje);

void enviar_mensaje_suscriptores(t_mq* cola);

/** Crea el paquete para manda en funcion del mensaje */
void enviar_mensaje(t_mensaje* mensaje, int cliente);

void* serializar_paquete(t_paquete* paquete, int bytes);

// Falta definirlo bien
int asignar_id_univoco();

#endif PROTOCOLO_H_
