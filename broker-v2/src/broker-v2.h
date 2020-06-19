#ifndef BROKER_H_
#define BROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include "conexion.h"
#include "constructor.h"

void iniciar_broker(void);
void terminar_broker(void);

#endif /* BROKER_H_ */
