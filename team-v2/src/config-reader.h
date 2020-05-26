#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

#include<stdio.h>
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/list.h>

typedef enum
{
	FIFO = 0,
	RR = 1,
	SJFCD = 2,
	SJFSD = 3,
	ALGORITMO_DESCONOCIDO = 4

} enum_algoritmo_planificacion;

t_config* config;
int tiempo_reconexion;
int retardo_ciclo_cpu;
enum_algoritmo_planificacion algoritmo_planificacion;
int quantum;
int estimacion_inicial;
char* ip_broker;
char* puerto_broker;
char* log_file;

void iniciar_config(void);
void destruir_config(void);

t_list* crear_t_list(char** array);
t_list* leer_posiciones_entrenadores(void);
t_list* leer_pokemon_entrenadores(void);
t_list* leer_objetivos_entrenadores(void);

int leer_tiempo_reconexion(void);
int leer_retardo_ciclo_cpu(void);
enum_algoritmo_planificacion leer_algoritmo_planificacion(void);
int leer_quantum(void);
int leer_estimacion_inicial(void);
char* leer_ip_broker(void);
char* leer_puerto_broker(void);
char* leer_log_file(void);

void setear_tiempo_reconexion(void);
void setear_retardo_ciclo_cpu(void);
void setear_algoritmo_planificacion(void);
void setear_quantum(void);
void setear_estimacion_inicial(void);
void setear_ip_broker(void);
void setear_puerto_broker(void);
void setear_log_file(void);

#endif /* CONFIGREADER_H_ */
