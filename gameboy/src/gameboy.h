#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include "conexion.h"

void iniciar_gameBoy(void);
void terminar_gameBoy(void);
void gestionar_envio_appeared(char* argv[], int argc);
void gestionar_envio_get(char* argv[], int argc);
void gestionar_envio_new(char* argv[], int argc);
void gestionar_envio_catch(char* argv[], int argc);
void gestionar_envio_caught(char* argv[]);
void gestionar_suscriptor(char* argv[]);
void esperar_y_finalizar(int tiempo);
int cola_mensajes(char* nombre_cola);
int tiene_cantidad_correcta_argumentos(int argc ,char *argv[]);

#endif /* GAMEBOY_H_ */
