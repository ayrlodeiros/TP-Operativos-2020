#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

#include<stdio.h>
#include<commons/config.h>

t_config* config;
char* ip_broker;
int puerto_broker;
char* ip_team;
int puerto_team;
char* ip_gamecard;
int puerto_gamecard;
char* log_file;
char* mi_log_file;

void iniciar_config(void);
void destruir_config(void);


void setear_ip_broker(void);
void setear_puerto_broker(void);
void setear_ip_team(void);
void setear_puerto_team(void);
void setear_ip_gamecard(void);
void setear_puerto_gamecard(void);

void setear_log_file(void);
void setear_mi_log_file(void);

char* leer_ip_broker(void);
int leer_puerto_broker(void);
char* leer_ip_team(void);
int leer_puerto_team(void);
char* leer_ip_gamecard(void);
int leer_puerto_gamecard(void);

char* leer_log_file(void);
char* leer_mi_log_file(void);


#endif /* CONFIGREADER_H_ */
