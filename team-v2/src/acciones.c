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

	loggear_movimiento(entrenador);
	//Ver como hacer para leer una unica vez el retardo
	sleep(leer_retardo_ciclo_cpu());
}
//Logea en el logger principal el movimiento que realizo un entrenador
//Solo usar dsp de que un entrenador cambio de posicion
void loggear_movimiento(entrenador* entrenador) {
<<<<<<< HEAD
	log_info(nuestro_log, string_from_format("Entrenador moviendose a la posicion: %d|%d.", entrenador->posicion->posicion_x, entrenador->posicion->posicion_y));
	log_info(logger, string_from_format("2. Entrenador moviendose a la posicion: %d|%d.", entrenador->posicion->posicion_x, entrenador->posicion->posicion_y));
=======
	log_info(nuestro_log, "2. Entrenador %d moviendose a la posicion: %d|%d.", entrenador->id, entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
	log_info(logger, "2. Entrenador %d moviendose a la posicion: %d|%d.", entrenador->id, entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
>>>>>>> develop
}

void ejecutar(entrenador* entrenador){

	accion* accion_a_ejecutar = list_remove(entrenador->acciones, 0);

	sumar_cpu_usado(entrenador, accion_a_ejecutar->cpu_requerido);

	// Se usa el hilo del entrenador para ejecutar la accion
	pthread_create(&(entrenador->hilo), NULL, accion_a_ejecutar->funcion, entrenador);
	pthread_join(entrenador->hilo, NULL);

	free(accion_a_ejecutar);
}

int cpu_restante_entrenador (entrenador* entrenador_a_ejecutar){
	return entrenador_a_ejecutar->cpu_disponible - entrenador_a_ejecutar->cpu_usado;
}

void sumar_cpu_usado(entrenador* entrenador, int cantidad) {
	entrenador->cpu_usado += cantidad;
}

void ejecutar(entrenador* entrenador){

	accion* accion_a_ejecutar = queue_pop(entrenador->acciones);

	sumar_cpu_usado(entrenador, accion_a_ejecutar->cpu_requerido);

	//TODO SACAR ESTOS LOGS
	log_info(nuestro_log, string_from_format("Cpu disponible del entrenador: %d", entrenador->cpu_disponible));
	log_info(nuestro_log, string_from_format("Cpu usado del entrenador: %d", entrenador->cpu_usado));

	pthread_create(&(entrenador->hilo), NULL, accion_a_ejecutar->funcion, entrenador);
	//TODO VER ESTO
	pthread_join(entrenador->hilo, NULL);

	free(accion_a_ejecutar);
}

int cpu_restante_entrenador (entrenador* entrenador_a_ejecutar){
	return entrenador_a_ejecutar->cpu_disponible - entrenador_a_ejecutar->cpu_usado;
}

void sumar_cpu_usado(entrenador* entrenador, int cantidad) {
	entrenador->cpu_usado += cantidad;
}
