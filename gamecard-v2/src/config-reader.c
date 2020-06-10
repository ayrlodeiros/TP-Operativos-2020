#include "config-reader.h"


void iniciar_config(void){
	config = config_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/gamecard.config");
	setear_tiempo_de_reintento_conexion();
	setear_tiempo_de_reintento_operacion();
	setear_tiempo_retardo_operacion();
	setear_punto_montaje_tallgrass();
	setear_ip_broker();
	setear_puerto_broker();
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

void setear_puerto_broker(void){
	puerto_broker = config_get_int_value(config, "PUERTO_BROKER");
}
