#include "config-reader.h"


void iniciar_config(void){
<<<<<<< HEAD
	config = config_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/gamecard.config");
=======
	config = config_create("../src/resources/gamecard.config");
>>>>>>> develop
	setear_tiempo_de_reintento_conexion();
	setear_tiempo_de_reintento_operacion();
	setear_tiempo_retardo_operacion();
	setear_punto_montaje_tallgrass();
	setear_ip_broker();
	setear_puerto_broker();
<<<<<<< HEAD
=======
	setear_id_modulo();
	setear_ip_gamecard();
	setear_puerto_gamecard();
	setear_path_logger();
	setear_path_nuestro_logger();
>>>>>>> develop
}

void destruir_config(void) {
	config_destroy(config);
}

int leer_tiempo_de_reintento_conexion(void){
	return tiempo_de_reintento_conexion;
}

int leer_tiempo_de_reintento_operacion(void){
	return tiempo_de_reintento_operacion;
}

int leer_tiempo_retardo_operacion(void){
	return tiempo_retardo_operacion;
}

char* leer_punto_montaje_tallgrass(void){
	return punto_montaje_tallgrass;
}

char* leer_ip_broker(void){
	return ip_broker;
}

int leer_puerto_broker(void){
	return puerto_broker;
}

<<<<<<< HEAD
=======
int leer_id_modulo(void) {
	return id_modulo;
}

char* leer_ip_gamecard(void){
	return ip_gamecard;
}

int leer_puerto_gamecard(void){
	return puerto_gamecard;
}

char* leer_path_logger(){
	return path_logger;
}

char* leer_path_nuestro_logger(){
	return path_nuestro_logger;
}

>>>>>>> develop
void setear_tiempo_de_reintento_conexion(void){
	tiempo_de_reintento_conexion = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
}

void setear_tiempo_de_reintento_operacion(void){
	tiempo_de_reintento_operacion = config_get_int_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
}

void setear_tiempo_retardo_operacion(void){
	tiempo_retardo_operacion = config_get_int_value(config, "TIEMPO_RETARDO_OPERACION");
}

void setear_punto_montaje_tallgrass(void){
	punto_montaje_tallgrass = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
}

void setear_ip_broker(void){
	ip_broker = config_get_string_value(config, "IP_BROKER");
}

<<<<<<< HEAD
void setear_puerto_broker(void){
	puerto_broker = config_get_int_value(config, "PUERTO_BROKER");
}
=======
void setear_path_logger(void){
	path_logger = config_get_string_value(config, "LOG_FILE");
}

void setear_path_nuestro_logger(void){
	path_nuestro_logger = config_get_string_value(config, "NUESTRO_LOG_FILE");
}

void setear_puerto_broker(void){
	puerto_broker = config_get_int_value(config, "PUERTO_BROKER");
}

void setear_id_modulo(void){
	id_modulo = config_get_int_value(config, "ID_MODULO");
}

void setear_ip_gamecard(void){
	ip_gamecard = config_get_string_value(config, "IP_GAMECARD");
}

void setear_puerto_gamecard(void){
	puerto_gamecard = config_get_int_value(config, "PUERTO_GAMECARD");
}
>>>>>>> develop
