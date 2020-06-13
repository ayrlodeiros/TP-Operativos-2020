#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include "config-reader.h"
#include "conexion.h"
//#include "protocolo.h"

t_log* logger;
t_log* mi_log;

void iniciar_gameBoy(void);
void terminar_gameBoy(void);
void gestionar_envio_appeared(char* argv[]);
void gestionar_envio_get(char* argv[]);
void gestionar_envio_new(char* argv[]);
void gestionar_envio_catch(char* argv[]);
void gestionar_envio_caught(char* argv[]);

#endif /* GAMEBOY_H_ */
