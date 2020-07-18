#ifndef CONSTRUCTOR_H_
#define CONSTRUCTOR_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<netdb.h>
#include<pthread.h>
#include<string.h>
#include <time.h>
#include<inttypes.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/queue.h>
#include<commons/collections/list.h>
#include "config-reader.h"

/* ADMINISTRACION DE MEMORIA */
void* memoria_principal;
t_list* lista_particiones;
int ultima_pos;

/** VG Para manejar ids mensajes */
int contador_ids_mensaje;


/*** ADMINISTRACION DE MEMORIA ESTRUCTURAS ***/
typedef struct{
	int inicio;
	int fin;
	int tamanio_ocupado;
	uint64_t tiempo_ingreso;
	uint64_t ult_vez_usado;
	bool libre;
}t_particion_dinamica;

typedef struct{
	t_particion_dinamica* particion;
	void* memoria;
}t_struct_temporal;

typedef struct{
	int inicio;
	int fin;
	int potencia_de_dos;
	uint64_t tiempo_ingreso;
	uint64_t ult_vez_usado;
	bool libre;
}t_particion_bs;


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
	t_list* cola;
	t_list* suscriptores; /** Lo cambie a una estructura suscriptor definida en protocolo.h*/
	pthread_mutex_t lock;
}t_mq;

/***********************************/

/* Estructura mensaje*/
typedef struct
{
	int size;
	void* stream;
}
t_buffer;

typedef struct{
	uint32_t pos;
	uint32_t tamanio;
}
t_pos_memoria;


/** Es la estructura con la que se va a guardar los mensajes en la memoria interna del broker, contiene mas informacion */
/** ESTRUCTURA MENSAJE */
typedef struct{
	int id;
	int id_cor;
	mq_nombre cola;
	/** Todavia no estoy seguro si hago dos listas de suscriptores (a los que ya se envio y los que falta recibir confirmacion),
	o una sola lista de una estrucutura que tenga el sucriptor y su estado */
	t_list* suscriptores_env;
	t_list* suscriptores_conf;
	t_pos_memoria* pos_en_memoria;
}
t_mensaje;

/**  Es la estructura con la que se va a enviar los mensajes a los modulos */
typedef struct{
	int id;
	int id_cor;
	t_buffer* buffer;
}
t_paquete;

/**  Estructura de suscriptor*/
typedef struct{
	int identificador;
	int conexion;
}
suscriptor_t;

/* Estructura de hilo enviar_mensaje / ACK*/
typedef struct{
	t_mensaje* mensaje;
	suscriptor_t* suscriptor;
}
aux_msj_susc;


/** Define el identificador del modulo*/
typedef enum{
	TEAM = 1,
	GAMEBOY = 2,
	GAMECARD = 3
}
modulo_code;

/** Define el identificador de la operacion  */
typedef enum
{
	MENSAJE=1,
	SUSCRIPCION=2
}
op_code;

/*Lista global de mensajes, tendria las estructuras de todos los mensajes */
t_list* lista_global_msjs;
pthread_mutex_t mutex_lista_msjs;


/** LOGS */
t_log* logger;
t_log* mi_log;
t_log* dump;

/* VG Colas de mensajes */
t_mq* get_mq;
t_mq* localized_mq;
t_mq* catch_mq;
t_mq* caught_mq;
t_mq* new_mq;
t_mq* appeared_mq;

/** Semaforos **/
pthread_mutex_t mutex_memoria_principal;
pthread_mutex_t mutex_id;

//Por ahora queda uno generico, dsp seguramente hay que hacer 6 (GET,NEW,APPEARED,LOCALIZED,CAUGHT,CATCH)
pthread_mutex_t mutex_agregar_msj_a_cola;


void iniciar_funcionalidades();

/* Dump de memoria */
void fecha_dump(void);
void iniciar_signal_handler();
void signal_handler(int signo);
char* obtener_fecha();
void dump_particion(int posicion,int inicio, int fin,bool libre,uint64_t lru);

/** Metodos para crear las colas de mensajes */
void inicializar_semaforos();
void inicializar_message_queues(void);
void esperar_mensaje_en_cola(t_mq* t_mq);
void liberar_message_queues(void);
void crear_get_mq(void);
void crear_localized_mq(void);
void crear_catch_mq(void);
void crear_caught_mq(void);
void crear_new_mq(void);
void crear_appeared_mq(void);

void liberar_mq(t_mq* mq);
void liberar_get_mq(void);
void liberar_localized_mq(void);
void liberar_catch_mq(void);
void liberar_caught_mq(void);
void liberar_new_mq(void);
void liberar_appeared_mq(void);


/* Lista GLobal de mensajes */
void agregar_a_lista_global(t_mensaje* mensaje);

/* Metodos estructura de mensajes */

t_mensaje* crear_mensaje(void* buffer,int tamanio,mq_nombre cola,int id_correlativo);

void enviar_mensaje_suscriptores(t_mq* cola);

/* Metodos estructura suscriptores */
suscriptor_t* crear_suscriptor(int conexion_suscriptor,int id_modulo);

/* Verifico para una lista de suscriptores si el msj ya fue enviado */
bool msj_enviado_a_suscriptor(int id_suscriptor,t_list* suscriptores_conf);

/* Metodos IDs Mensajes */
void iniciar_contador_ids_mensaje();
// Aumenta el valor de la variable global en 1 y devuelve eso
int asignar_id_univoco();

/* Inicia la lista de msjs global*/
void iniciar_list_global();

/** Crea el paquete para manda en funcion del mensaje */
void enviar_mensaje(aux_msj_susc* aux);

void* serializar_paquete(t_paquete* paquete, int bytes);

void recibir_ACK(aux_msj_susc* msj_y_susc);

void add_sub_lista_env_msj(t_mensaje* mensaje,suscriptor_t* suscriptor);
void add_sub_lista_conf_msj(t_mensaje* mensaje,suscriptor_t* suscriptor);



/* ADMINISTRACION DE MEMORIA */

void iniciar_memoria_principal();
int guardar_mensaje_en_memoria(int tamanio, void*buffer);
void almacenar_en_memoria(int tamanio, void* buffer, int posicion);

// Normal
int obtener_posicion_normal();

// Dinamica
void inicializar_lista_particiones();
int obtener_posicion_particiones(int tamanio);
bool estaOcupado(void* elemento);
bool noEstaOcupado(void* elemento);
void borrarParticion(void* elemento);
void compactacion();
void llenar_memoria_principal(int posicion, int tamanio, void* mensaje);
t_list* obtener_particiones_ocupadas();
t_list* crear_list_temporal(t_list* particiones);
void liberar_particion();
int algoritmo_reemplazo_fifo(void);
int algoritmo_reemplazo_lru(void);
void consolidar(int pos_particion);
bool particion_libre_a_la_izquierda(int posicion,int inicio_part_liberada);
bool particion_libre_a_la_derecha(int posicion,int fin_part_liberada);
int buscar_particion_libre(int tamanio);
int algoritmo_best_fit(int tamanio);
int algoritmo_first_fit(int tamanio);
t_particion_dinamica* crear_particion_dinamica_libre();
int llenar_y_realizar_nueva_particion(t_particion_dinamica* particion,int tamanio,int posicion_en_lista);
int llenar_particion(t_particion_dinamica* particion, int tamanio);
bool esta_libre(t_particion_dinamica* particion);
int diferencia_tamanio_particion(t_particion_dinamica* particion,int tamanio_msj);
int tamanio_particion(t_particion_dinamica* particion);

// Buddy System
void inicializar_lista_bs();
int obtener_posicion_bs(int tamanio);
int obtener_potencia_de_dos_mas_cercana(int valor);
int obtener_posicion_particion_mas_cercana(int potencia_de_dos);
void liberar_y_consolidar();
int liberar_una_particion();
int obtener_posicion_de_particion_liberada_fifo();
int obtener_posicion_de_particion_liberada_lru();
int evaluar_consolidacion(int posicion_buddy_1);
void consolidar_buddies(int posicion_buddy_a_eliminar, t_particion_bs* buddy_a_mantener);
t_particion_bs* particionar_y_obtener_particion(int posicion_a_particionar, int potencia_de_dos_deseada);
int potencia(int base, int exponente);

/* Esto es para cuando se accede a la memoria de una particion, saber en que momento fue utilizada para el algoritmo LRU */
uint64_t timestamp(void);
void borrar_msj_mp(int posicion);
void destruir_t_mensaje(t_mensaje* mensaje);
void actualizar_ultima_vez_usado_particion(t_mensaje* mensaje);
void actualizar_ultima_vez_dinamica(t_mensaje* mensaje);
void actualizar_ultima_vez_bs(t_mensaje* mensaje);

void int_handler(int signal);
void iniciar_sigint_handler(void);
void dump_solo_particion(void);

#endif //CONSTRUCTOR_H_
