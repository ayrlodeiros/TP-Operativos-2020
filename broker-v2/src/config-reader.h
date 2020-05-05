#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_
#include<stdio.h>
#include<commons/config.h>
#include<commons/collections/list.h>

t_config* config;
int TAMANO_MEMORIA;
int TAMANO_MINIMO_PARTICION;
char* ALGORITMO_MEMORIA;
char* ALGORITMO_REEMPLAZO;
char* ALGORITMO_PARTICION_LIBRE;
char* IP_BROKER;
char* PUERTO_BROKER;
int FRECUENCIA_COMPACTACION;
char* LOG_FILE;

void iniciar_config(void);
void destruir_config(void);

void setear_tamano_memoria(void);
void setear_tamano_minimo_particion(void);
void setear_algoritmo_memoria(void);
void setear_algoritmo_reemplazo(void);
void setear_algoritmo_particion_libre(void);
void setear_ip_broker(void);
void setear_puerto_broker(void);
void setear_frecuencia_compactacion(void);
void setear_log_file(void);

int leer_tamano_memoria(void);
int leer_tamano_minimo_particion(void);
char* leer_algoritmo_memoria(void);
char* leer_algoritmo_reemplazo(void);
char* leer_algoritmo_particion_libre(void);
char* leer_ip_broker(void);
char* leer_puerto_broker(void);
int leer_frecuencia_compactacion(void);
char* leer_log_file(void);


#endif /* CONFIGREADER_H_ */
