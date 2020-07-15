#include "config-reader.h"


void iniciar_config(void){
	config = config_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gameboy-v2/src/resources/gameBoy.config");
	setear_ip_broker();
	setear_ip_team();
	setear_ip_gamecard();
	setear_puerto_broker();
	setear_puerto_team();
	setear_puerto_gamecard();
}

void destruir_config(void){
	config_destroy(config);
}


void setear_ip_broker(void){
	ip_broker = config_get_string_value(config,"IP_BROKER");
}
void setear_ip_team(void){
	ip_team = config_get_string_value(config,"IP_TEAM");
}
void setear_ip_gamecard(void){
	ip_gamecard = config_get_string_value(config,"IP_GAMECARD");
}
void setear_puerto_broker(void){
	puerto_broker = config_get_int_value(config,"PUERTO_BROKER");
}
void setear_puerto_team(void){
	puerto_team = config_get_int_value(config,"PUERTO_TEAM");
}
void setear_puerto_gamecard(void){
	puerto_gamecard = config_get_int_value(config,"PUERTO_GAMECARD");
}

char* leer_ip_broker(void){
	return ip_broker;
}
char* leer_ip_team(void){
	return ip_team;
}
char* leer_ip_gamecard(void){
	return ip_gamecard;
}
int leer_puerto_broker(void){
	return puerto_broker;
}
int leer_puerto_team(void){
	return puerto_team;
}
int leer_puerto_gamecard(void){
	return puerto_gamecard;
}
