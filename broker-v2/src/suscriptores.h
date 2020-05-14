#ifndef SUSCRIPTORES_H_
#define SUSCRIPTORES_H_

#include <stdio.h>
#include <stdlib.h>
#include "message-queue.h"
#include "utils.h"

typedef struct{
	int conexion;
	modulo_code modulo;
}suscriptor_t;

void switch_mensaje(int codigo_cola,int conexion);
void switch_suscripcion(int codigo_cola,int conexion,modulo_code id_modulo);
suscriptor_t* crear_suscriptor(int conexion_suscriptor,modulo_code codigo_suscriptor);
#endif
