#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_
#include<stdio.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/string.h>

typedef enum{
	PARTICIONES = 1,
	BS = 2,
	NORMAL = 3
}
algoritmo_memoria;

typedef enum{
	FIFO = 1,
	LRU = 2
}
algoritmo_reemplazo;

typedef enum{
	FF = 1,
	BF = 2
}
algoritmo_particion_libre;

t_config* config;
int TAMANO_MEMORIA;
int TAMANO_MINIMO_PARTICION;
algoritmo_memoria ALGORITMO_MEMORIA;
algoritmo_reemplazo ALGORITMO_REEMPLAZO;
algoritmo_particion_libre ALGORITMO_PARTICION_LIBRE;
char* IP_BROKER;
int PUERTO_BROKER;
int FRECUENCIA_COMPACTACION;
char* PATH_ARCHIVO_DUMP;
char* LOG_FILE;
char* NUESTRO_LOG_FILE;

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
void setear_path_archivo_dump(void);
void setear_log_file(void);
void setear_nuestro_log_file(void);

int leer_tamano_memoria(void);
int leer_tamano_minimo_particion(void);
algoritmo_memoria leer_algoritmo_memoria(void);
algoritmo_reemplazo leer_algoritmo_reemplazo(void);
algoritmo_particion_libre leer_algoritmo_particion_libre(void);
char* leer_ip_broker(void);
int leer_puerto_broker(void);
int leer_frecuencia_compactacion(void);
char* leer_path_archivo_dump(void);
char* leer_log_file(void);
char* leer_nuestro_log_file(void);


#endif /* CONFIGREADER_H_ */
