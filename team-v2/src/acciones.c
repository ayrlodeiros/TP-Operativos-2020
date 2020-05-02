#include "acciones.h"

//Mover al entrenador una posicion para arriba
void moverse_arriba(entrenador* entrenador) {
	entrenador->posicion->posicion_y += 1;
	registrar_movimiento(entrenador);
}
//Mover al entrenador una posicion para abajo
void moverse_abajo(entrenador* entrenador) {
	entrenador->posicion->posicion_y -= 1;
	registrar_movimiento(entrenador);
}
//Mover al entrenador una posicion para izquierda
void moverse_izquierda(entrenador* entrenador) {
	entrenador->posicion->posicion_x -= 1;
	registrar_movimiento(entrenador);
}
//Mover al entrenador una posicion para derecha
void moverse_derecha(entrenador* entrenador) {
	entrenador->posicion->posicion_x += 1;
	registrar_movimiento(entrenador);
}
//Comportamiento en comun que tienen los movimientos
void registrar_movimiento(entrenador* entrenador) {
	restar_cpu_disponible(entrenador, 1);
	sumar_cpu_usado(entrenador, 1);
	loggear_movimiento(entrenador);
	//Ver como hacer para leer una unica vez el retardo
	sleep(leer_retardo_ciclo_cpu());
}
//Logea en el logger principal el movimiento que realizo un entrenador
//Solo usar dsp de que un entrenador cambio de posicion
void loggear_movimiento(entrenador* entrenador) {
	log_info(logger, string_from_format("2. Entrenador moviendose a la posicion: %d|%d.", entrenador->posicion->posicion_x, entrenador->posicion->posicion_y));
}


//Realiza un intercambio entre dos entrenadores que estaban bloqueados
void intercambiar(entrenador* entrenador1, entrenador* entrenador2) {

}
