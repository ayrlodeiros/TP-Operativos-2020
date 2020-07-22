#include "config-reader.h"


void iniciar_config(void){
	config = config_create("../src/resources/broker.config");
	setear_tamano_memoria();
	setear_tamano_minimo_particion();
	setear_algoritmo_memoria();
	setear_algoritmo_reemplazo();
	setear_algoritmo_particion_libre();
	setear_ip_broker();
	setear_puerto_broker();
	setear_frecuencia_compactacion();
	setear_path_archivo_dump();
	setear_log_file();
	setear_nuestro_log_file();
}

void destruir_config(){
	config_destroy(config);
}

void setear_tamano_minimo_particion(void){
	//todo volver a setear los valores
	TAMANO_MINIMO_PARTICION = 4;
			//config_get_int_value(config, "TAMANO_MINIMO_PARTICION");
}

void setear_tamano_memoria(void){
	TAMANO_MEMORIA = 64;
			//config_get_int_value(config,"TAMANO_MEMORIA");
}
void setear_algoritmo_memoria(void){
	ALGORITMO_MEMORIA = 2;
	/*
	char* algoritmo =
			config_get_string_value(config,"ALGORITMO_MEMORIA");
	if(string_equals_ignore_case(algoritmo,"PARTICIONES")) ALGORITMO_MEMORIA = 1;
	else if(string_equals_ignore_case(algoritmo,"BS")) ALGORITMO_MEMORIA = 2;
		else
		ALGORITMO_MEMORIA = 3;
	*/
}
void setear_algoritmo_reemplazo(void){
	ALGORITMO_REEMPLAZO = 2;
			/*
	char* algoritmo = config_get_string_value(config,"ALGORITMO_REEMPLAZO");
		if(string_equals_ignore_case(algoritmo,"FIFO")) ALGORITMO_REEMPLAZO = 1;
		else
			ALGORITMO_REEMPLAZO = 2;
*/
}
void setear_algoritmo_particion_libre(void){
	ALGORITMO_PARTICION_LIBRE = 1;
	/*
	char* algoritmo = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
			if(string_equals_ignore_case(algoritmo,"FF")) ALGORITMO_PARTICION_LIBRE = 1;
			else
				ALGORITMO_PARTICION_LIBRE = 2;
				*/
}
void setear_ip_broker(void){
	IP_BROKER = "127.0.0.1";
			//config_get_string_value(config,"IP_BROKER");
}
void setear_puerto_broker(void){
	PUERTO_BROKER = 6009;
			//config_get_int_value(config,"PUERTO_BROKER");
}
void setear_frecuencia_compactacion(void){
	FRECUENCIA_COMPACTACION = 1;
			//config_get_int_value(config,"FRECUENCIA_COMPACTACION");
}
void setear_path_archivo_dump(void){
	PATH_ARCHIVO_DUMP = "/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2/src/resources/dump_file.txt";
			//config_get_string_value(config,"PATH_ARCHIVO_DUMP");
}
void setear_log_file(void){
	LOG_FILE = "/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2/src/resources/log_broker.txt";
			//config_get_string_value(config,"LOG_FILE");
}
void setear_nuestro_log_file(void){
	NUESTRO_LOG_FILE ="/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2/src/resources/mi_log_broker.txt";
			//config_get_string_value(config,"NUESTRO_LOG_FILE");
}

int leer_tamano_memoria(void){
	return TAMANO_MEMORIA;
}
int leer_tamano_minimo_particion(void){
	return TAMANO_MINIMO_PARTICION;
}
algoritmo_memoria leer_algoritmo_memoria(void){
	return ALGORITMO_MEMORIA;
}
algoritmo_reemplazo leer_algoritmo_reemplazo(void){
	return ALGORITMO_REEMPLAZO;
}
algoritmo_particion_libre leer_algoritmo_particion_libre(void){
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
char* leer_path_archivo_dump(void){
	return PATH_ARCHIVO_DUMP;
}
char* leer_log_file(void){
	return LOG_FILE;
}
char* leer_nuestro_log_file(void){
	return NUESTRO_LOG_FILE;
}

