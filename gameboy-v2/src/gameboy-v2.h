#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

#include "config-reader.h"
#include "conexion.h"

t_log* logger;
t_log* mi_log;

void iniciar_gameBoy(void);
void terminar_gameBoy(void);


#endif /* GAMEBOY_H_ */
