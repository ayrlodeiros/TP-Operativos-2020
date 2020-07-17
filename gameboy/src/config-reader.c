#include "config-reader.h"


void iniciar_config(void){
	config = config_create("../src/resources/gameBoy.config");
	setear_ip_broker();
	setear_ip_team();
	setear_ip_gamecard();
	setear_puerto_broker();
	setear_puerto_team();
	setear_puerto_gamecard();
	setear_log_file();
	setear_mi_log_file();
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
void setear_log_file() {
	log_file = config_get_string_value(config,"LOG_FILE");
}
void setear_mi_log_file() {
	mi_log_file = config_get_string_value(config,"MI_LOG_FILE");
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
char* leer_log_file(void){
	return log_file;
}
char* leer_mi_log_file(void){
	return mi_log_file;
}
