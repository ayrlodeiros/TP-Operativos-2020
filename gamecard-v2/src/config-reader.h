/*
 * config-reader.h
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_


#include<stdio.h>
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/list.h>


t_config* config;
int tiempo_de_reintento_conexion;
int tiempo_de_reintento_operacion;
int tiempo_retardo_operacion;
char* punto_montaje_tallgrass;
char* ip_broker;
int puerto_broker;
<<<<<<< HEAD
=======
int id_modulo;
char* ip_gamecard;
int puerto_gamecard;
char* path_logger;
char* path_nuestro_logger;
>>>>>>> develop

void iniciar_config(void);
void destruir_config(void);

int leer_tiempo_de_reintento_conexion(void);
int leer_tiempo_de_reintento_operacion(void);
int leer_tiempo_retardo_operacion(void);
char* leer_punto_montaje_tallgrass(void);
char* leer_ip_broker(void);
int leer_puerto_broker(void);
<<<<<<< HEAD
=======
int leer_id_modulo(void);
char* leer_ip_gamecard(void);
int leer_puerto_gamecard(void);
char* leer_path_logger();
char* leer_path_nuestro_logger();
>>>>>>> develop

void setear_tiempo_de_reintento_conexion(void);
void setear_tiempo_de_reintento_operacion(void);
void setear_tiempo_retardo_operacion(void);
void setear_punto_montaje_tallgrass(void);
void setear_ip_broker(void);
void setear_puerto_broker(void);
<<<<<<< HEAD
=======
void setear_id_modulo(void);
void setear_ip_gamecard(void);
void setear_puerto_gamecard(void);
void setear_path_logger(void);
void setear_path_nuestro_logger(void);
>>>>>>> develop


#endif /* CONFIG_READER_H_ */
