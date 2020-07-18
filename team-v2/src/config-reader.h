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
double alpha;
double estimacion_inicial;
char* ip_broker;
char* puerto_broker;
char* log_file;
char* nuestro_log_file;
int id_modulo;
char* ip_team;
char* puerto_team;

void iniciar_config(void);
void destruir_config(void);

t_list* crear_t_list(char** array);
int tamanio_de_lista(char** un_array);
t_dictionary* crear_t_dictionary(char** array);
t_list* leer_posiciones_entrenadores(void);
t_list* leer_pokemon_entrenadores(void);
t_list* leer_objetivos_entrenadores(void);

int leer_tiempo_reconexion(void);
int leer_retardo_ciclo_cpu(void);
enum_algoritmo_planificacion leer_algoritmo_planificacion(void);
int leer_quantum(void);
double leer_alpha(void);
double leer_estimacion_inicial(void);
char* leer_ip_broker(void);
char* leer_puerto_broker(void);
char* leer_log_file(void);
char* leer_nuestro_log_file(void);
int leer_id_modulo(void);
char* leer_ip_team(void);
char* leer_puerto_team(void);

void setear_tiempo_reconexion(void);
void setear_retardo_ciclo_cpu(void);
void setear_algoritmo_planificacion(void);
void setear_quantum(void);
void setear_alpha(void);
void setear_estimacion_inicial(void);
void setear_ip_broker(void);
void setear_puerto_broker(void);
void setear_log_file(void);
void setear_nuestro_log_file(void);
void setear_id_modulo(void);
void setear_ip_team(void);
void setear_puerto_team(void);

#endif /* CONFIGREADER_H_ */
