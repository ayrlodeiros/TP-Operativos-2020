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
	log_info(nuestro_log, string_from_format("2. Entrenador moviendose a la posicion: %d|%d.", entrenador->posicion->posicion_x, entrenador->posicion->posicion_y));
	log_info(logger, string_from_format("2. Entrenador moviendose a la posicion: %d|%d.", entrenador->posicion->posicion_x, entrenador->posicion->posicion_y));
}

//Realiza la llamada al catch
//POR AHORA ESTA EN utils.c COMO atrapar_pokemon
void realizar_catch(entrenador* entrenador) {

	//TODO realizar catch
	//Deberia crear una conexion con el broker, enviar el catch con el nombre del pokemon y ...
	//... esperar el ID de respuesta del broker para luego ir a esperar ese ID en la suscripcion del CAUGHT.
	//Ademas deberia dejar al entrenador en estado BLOCK_CATCHING.
	//Pensar como leer tmb cual ID corresponde a cada entrenador que esta en BLOCK_CATCHING.
}


//Realiza un intercambio entre dos entrenadores que estaban bloqueados
void intercambiar(entrenador* entrenador1, entrenador* entrenador2) {
	//TODO realizar intercamiar
	//Vamos a tener que ver que pokemons no necesita el entrenador1, que pokemons no necesita el entrenador2,...
	//... ver si alguno de los dos necesita lo que el otro no necesita y:
	//Si ambos necesitan lo que el otro no necesita: ambos van a recibir algo que necesitan
	//Si uno solo necesita lo que el otro no necesita: uno va a recibir lo que necesita y otro va a quedarse BLOCK
	//Si ninguno necesita lo que el otro no necesita: hacer el intercambio por cualquiera, pero quedarse esperando a que se BLOCKEE otro entrenador para intercambiar

	//Igual hay que ver bien como se va a trabajar a este metodo y cuando se lo va a llamar
	//Uno de los casos en el que se va a llamar es cuando un entrenador pase a BLOCK_DEADLOCK y ya haya alguno en ese estado tambien
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

	free(accion_a_ejecutar->funcion);
	free(accion_a_ejecutar->cpu_requerido);
	free(accion_a_ejecutar);
}

int cpu_restante_entrenador (entrenador* entrenador_a_ejecutar){
	return entrenador_a_ejecutar->cpu_disponible - entrenador_a_ejecutar->cpu_usado;
}

void sumar_cpu_usado(entrenador* entrenador, int cantidad) {
	entrenador->cpu_usado += cantidad;
}
