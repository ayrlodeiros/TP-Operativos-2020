#ifndef ACCIONES_H_
#define ACCIONES_H_

#include "constructor.h"

void moverse_arriba(entrenador* entrenador);
void moverse_abajo(entrenador* entrenador);
void moverse_izquierda(entrenador* entrenador);
void moverse_derecha(entrenador* entrenador);
void registrar_movimiento(entrenador* entrenador);
void loggear_movimiento(entrenador* entrenador);
<<<<<<< HEAD
void intercambiar(entrenador* entrenador1, entrenador* entrenador2);
=======
>>>>>>> develop
void ejecutar(entrenador* entrenador);
int cpu_restante_entrenador (entrenador* entrenador_a_ejecutar);
void sumar_cpu_usado(entrenador* entrenador, int cantidad);

#endif /* ACCIONES_H_ */
