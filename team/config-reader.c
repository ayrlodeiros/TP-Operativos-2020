#include "config-reader.h"

void iniciar_config(void){
	config = config_create("team.config");
}

t_config* get_config(void){
	return config;
}

void destruir_config(void) {
	config_destroy(config);
}

void leer_posiciones_entrenadores(void){

}
void leer_pokemon_entrenadores(void){

}
void leer_objetivos_entrenadores(void){

}

int leer_tiempo_reconexion(void){
	return config_get_int_value(config, "TIEMPO_RECONEXION");
}

int leer_retardo_ciclo_cpu(void){
	return config_get_int_value(config, "RETARDO_CICLO_CPU");
}

char* leer_algoritmo_planificacion(void){
	return config_get_string_value(config, "ALGORITMO_PLANIFICACION");
}

int leer_quantum(void){
	return config_get_int_value(config, "QUANTUM");
}

int leer_estimacion_inicial(void){
	return config_get_int_value(config, "ESTIMACION_INICIAL");
}

char* leer_ip_broker(void){
	return config_get_string_value(config, "IP_BROKER");
}

char* leer_puerto_broker(void){
	return config_get_string_value(config, "PUERTO_BROKER");
}

char* leer_log_file(void){
	return config_get_string_value(config, "LOG_FILE");
}
