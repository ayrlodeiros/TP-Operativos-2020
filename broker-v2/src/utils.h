#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>

#include<commons/log.h>
#include<commons/config.h>

t_log* logger;
t_log* mi_log;
/* Es para asegurarme que no se repitan los ids al asignar mensajes, ver si se puede aplicar algo mejor*/
int contador_ids_mensaje;

typedef enum{
	TEAM = 1,
	GAMEBOY = 2,
	GAMECARD = 3
}modulo_code;


typedef enum
{
	MENSAJE=1,
	SUSCRIPCION=2
}op_code;

void iniciar_contador_ids_mensaje();

#endif /* UTILS_H_ */
