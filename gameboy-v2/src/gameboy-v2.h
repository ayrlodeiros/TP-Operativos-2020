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
int cola_mensajes(char* nombre_cola);
t_new_pokemon* armar_new_pokemon(char* nombre, int posicion_x, int posicion_y, int cantidad);

#endif /* GAMEBOY_H_ */
