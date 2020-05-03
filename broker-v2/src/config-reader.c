#include "config-reader.h"


void iniciar_config(void){
	config = config_create("src/resources/broker.config");
	//setear_tamano_memoria();
	setear_tamano_minimo_particion();
	setear_algoritmo_memoria();
	setear_algoritmo_reemplazo();
	setear_algoritmo_particion_libre();
	setear_ip_broker();
	setear_puerto_broker();
	setear_frecuencia_compactacion();
	setear_log_file();
}

void destruir_config(){
	config_destroy(config);
}


void setear_tamano_memoria(void){
	TAMANO_MEMORIA = config_get_int_value(config,"TAMANO_MEMORIA");
}
void setear_tamano_minimo_particion(void){
	TAMANO_MINIMO_PARTICION = config_get_int_value(config, "TAMANO_MINIMO_PARTICION");
}
void setear_algoritmo_memoria(void){
	ALGORITMO_MEMORIA = config_get_string_value(config,"ALGORITMO_MEMORIA");
}
void setear_algoritmo_reemplazo(void){
	ALGORITMO_REEMPLAZO = config_get_string_value(config,"ALGORITMO_REEMPLAZO");
}
void setear_algoritmo_particion_libre(void){
	ALGORITMO_PARTICION_LIBRE = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
}
void setear_ip_broker(void){
	IP_BROKER = config_get_string_value(config,"IP_BROKER");
}
void setear_puerto_broker(void){
	PUERTO_BROKER = config_get_int_value(config,"PUERTO_BROKER");
}
void setear_frecuencia_compactacion(void){
	FRECUENCIA_COMPACTACION = config_get_int_value(config,"FRECUANCIA_COMPACTACION");
}
void setear_log_file(void){
	LOG_FILE = config_get_string_value(config,"LOG_FILE");
}

int leer_tamano_memoria(void){
	return TAMANO_MEMORIA;
}
int leer_tamano_minimo_particion(void){
	return TAMANO_MINIMO_PARTICION;
}
char* leer_algoritmo_memoria(void){
	return ALGORITMO_MEMORIA;
}
char* leer_algoritmo_reemplazo(void){
	return ALGORITMO_REEMPLAZO;
}
char* leer_algoritmo_particion_libre(void){
	return ALGORITMO_PARTICION_LIBRE;
}
char* leer_ip_broker(void){
	return IP_BROKER;
}
int leer_puerto_broker(void){
	return PUERTO_BROKER;
}
int leer_frecuencia_compactacion(void){
	return FRECUENCIA_COMPACTACION;
}
char* leer_log_file(void){
	return LOG_FILE;
}
