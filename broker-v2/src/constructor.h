#ifndef CONSTRUCTOR_H_
#define CONSTRUCTOR_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/queue.h>
#include<commons/collections/list.h>

/** VG Para manejar ids mensajes */
int contador_ids_mensaje;

/*** COLAS DE MENSAJES ESTRUCTURAS ***/

/* Define los nombres de las colas de mensaje */
typedef enum{
		GET = 1,
		LOCALIZED = 2,
		CATCH = 3,
		CAUGHT = 4,
		NEW = 5 ,
		APPEARED = 6
}mq_nombre;

/*  Define la estructura de una cola de mensajes  */
typedef struct{
	mq_nombre nombre;
	t_queue* cola;
	t_list* suscriptores; /** Lo cambie a una estructura suscriptor definida en protocolo.h*/

}t_mq;

/***********************************/

/* Estructura mensaje*/
typedef struct
{
	int size;
	void* stream;
}
t_buffer;

/** Es la estructura con la que se va a guardar los mensajes en la memoria interna del broker, contiene mas informacion */
/** todo ESTRUCTURA MENSAJE */
typedef struct{
	int id;
	int id_cor;
	mq_nombre cola;
	/** Todavia no estoy seguro si hago dos listas de suscriptores (a los que ya se envio y los que falta recibir confirmacion),
	o una sola lista de una estrucutura que tenga el sucriptor y su estado */
	t_list* suscriptores_env;
	t_list* suscriptores_conf;
	t_buffer* buffer;

}
t_mensaje;

/**  Es la estructura con la que se va a enviar los mensajes a los modulos */
typedef struct{
	int id;
	int id_cor;
	t_buffer* buffer;
}
t_paquete;

/** Define el identificador del modulo*/
typedef enum{
	TEAM = 1,
	GAMEBOY = 2,
	GAMECARD = 3
}
modulo_code;

/**  Estructura de suscriptor*/
typedef struct{
	int conexion;
	modulo_code modulo;
}
suscriptor_t;

/** Define el identificador de la operacion  */
typedef enum
{
	MENSAJE=1,
	SUSCRIPCION=2
}
op_code;

/** LOGS */
t_log* logger;
t_log* mi_log;


/* VG Colas de mensajes */
t_mq* get_mq;
t_mq* localized_mq;
t_mq* catch_mq;
t_mq* caught_mq;
t_mq* new_mq;
t_mq* appeared_mq;


/** Metodos para crear las colas de mensajes */
void inicializar_message_queues(void);
void liberar_message_queues(void);
void crear_get_mq(void);
void crear_localized_mq(void);
void crear_catch_mq(void);
void crear_caught_mq(void);
void crear_new_mq(void);
void crear_appeared_mq(void);

void liberar_get_mq(void);
void liberar_localized_mq(void);
void liberar_catch_mq(void);
void liberar_caught_mq(void);
void liberar_new_mq(void);
void liberar_appeared_mq(void);

/* Metodos estructura de mensajes */

t_mensaje* crear_mensaje(void* buffer,int tamanio,mq_nombre cola);

/* Metodos estructura suscriptores */
suscriptor_t* crear_suscriptor(int conexion_suscriptor,modulo_code codigo_suscriptor);

/* Metodos IDs Mensajes */
void iniciar_contador_ids_mensaje();

#endif CONSTRUCTOR_H_
