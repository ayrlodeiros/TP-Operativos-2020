#include "config-reader.h"


void iniciar_config(void){
	config = config_create("src/resources/gameBoy.config");
	setear_ip_broker();
	setear_ip_team();
	setear_ip_gamecard();
	setear_puerto_broker();
	setear_puerto_team();
	setear_puerto_gamecard();
}

void destruir_config(){
	config_destroy(config);
}


void setear_ip_broker(void){
	IP_BROKER = config_get_string_value(config,"IP_BROKER");
}
void setear_ip_team(void){
	IP_TEAM = config_get_string_value(config,"IP_TEAM");
}
void setear_ip_gamecard(void){
	IP_GAMECARD = config_get_string_value(config,"IP_GAMECARD");
}
void setear_puerto_broker(void){
	PUERTO_BROKER = config_get_string_value(config,"PUERTO_BROKER");
}
void setear_puerto_team(void){
	PUERTO_TEAM = config_get_string_value(config,"PUERTO_TEAM");
}
void setear_puerto_gamecard(void){
	PUERTO_GAMECARD = config_get_string_value(config,"PUERTO_GAMECARD");
}



char* leer_ip_broker(void){
	return IP_BROKER;
}
char* leer_ip_team(void){
	return IP_TEAM;
}
char* leer_ip_gamecard(void){
	return IP_GAMECARD;
}
char* leer_puerto_broker(void){
	return PUERTO_BROKER;
}
char* leer_puerto_team(void){
	return PUERTO_TEAM;
}
char* leer_puerto_gamecard(void){
	return PUERTO_GAMECARD;
}
