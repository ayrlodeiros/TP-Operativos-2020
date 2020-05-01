#ifndef ACCIONES_H_
#define ACCIONES_H_

#include "utils.h"

void moverse_arriba(entrenador* entrenador);
void moverse_abajo(entrenador* entrenador);
void moverse_izquierda(entrenador* entrenador);
void moverse_derecha(entrenador* entrenador);
void registrar_movimiento(entrenador* entrenador);
void loggear_movimiento(entrenador* entrenador);
void intercambiar(entrenador* entrenador1, entrenador* entrenador2);

#endif /* ACCIONES_H_ */
