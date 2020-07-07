#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include "conexion.h"

void iniciar_gameBoy(void);
void terminar_gameBoy(void);
void gestionar_envio_appeared(char* argv[]);
void gestionar_envio_get(char* argv[]);
void gestionar_envio_new(char* argv[]);
void gestionar_envio_catch(char* argv[]);
void gestionar_envio_caught(char* argv[]);
void gestionar_suscriptor(char* argv[]);
int cola_mensajes(char* nombre_cola);

#endif /* GAMEBOY_H_ */
