#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_
#include<stdio.h>
#include<commons/config.h>
#include<commons/collections/list.h>

t_config* config;
char* IP_BROKER;
char* PUERTO_BROKER;
char* IP_TEAM;
char* PUERTO_TEAM;
char* IP_GAMECARD;
char* PUERTO_GAMECARD;

void iniciar_config(void);
void destruir_config(void);


void setear_ip_broker(void);
void setear_puerto_broker(void);
void setear_ip_team(void);
void setear_puerto_team(void);
void setear_ip_gamecard(void);
void setear_puerto_gamecard(void);

void setear_log_file(void);

char* leer_ip_broker(void);
char* leer_puerto_broker(void);
char* leer_ip_team(void);
char* leer_puerto_team(void);
char* leer_ip_gamecard(void);
char* leer_puerto_gamecard(void);

char* leer_log_file(void);


#endif /* CONFIGREADER_H_ */
