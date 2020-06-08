#include "constructor.h"

//Se deberia ejecutar una sola vez en el team
void iniciar_variables_globales() {
	armar_entrenadores();
	armar_objetivo_global();
	pokemons_sin_entrenador = queue_create();
	entrenadores_ready = list_create();
	lista_ids_localized = list_create();
	lista_aux_pokemons = list_create();
	pthread_mutex_init(&lock_de_planificacion, NULL);
	pthread_mutex_init(&lock_de_entrenador_disponible, NULL);
	pthread_mutex_init(&mutex_entrenadores, NULL);
	pthread_mutex_init(&mutex_lista_ids_localized, NULL);

	pthread_mutex_lock(&lock_de_planificacion);
	pthread_mutex_lock(&lock_de_entrenador_disponible);

	funciona_broker=0;
}

//Se deberia ejecutar una sola vez, en el metodo inciar_variables_globales
void armar_entrenadores() {
	entrenadores = list_create();

	t_list* posiciones = leer_posiciones_entrenadores();
	t_list* pokemons = leer_pokemon_entrenadores();
	t_list* objetivos = leer_objetivos_entrenadores();

	for(int i=0; i< list_size(posiciones);i++){
		list_add(entrenadores, armar_entrenador(list_get(posiciones, i), list_get(pokemons, i), list_get(objetivos, i)));
	}

}

//Se deberia usar solo en armar_entrenadores
entrenador* armar_entrenador(char* posicion, char* pokemons, char* objetivos){
	entrenador* un_entrenador = malloc(sizeof(entrenador));
	char** lista_objetivos = string_split(objetivos,"|");
	t_list* lista_pokemons_objetivo = crear_t_list(lista_objetivos);

	un_entrenador->posicion = armar_posicion(posicion);
	un_entrenador->estado = NEW;
	un_entrenador->pokemons_adquiridos = crear_t_dictionary(string_split(pokemons,"|"));
	un_entrenador->pokemons_objetivo = crear_t_dictionary(lista_objetivos);
	un_entrenador->cant_maxima_pokemons = list_size(lista_pokemons_objetivo);
	list_destroy(lista_pokemons_objetivo);
	un_entrenador->cpu_usado = 0;
	un_entrenador->cpu_disponible = 0;
	un_entrenador->cpu_estimado_anterior = leer_estimacion_inicial();
	un_entrenador->cpu_estimado_restante = un_entrenador->cpu_estimado_anterior;
	un_entrenador->acciones = queue_create();

	return un_entrenador;
}

//Se deberia ejecutar una sola vez, en el metodo inciar_variables_globales
void armar_objetivo_global() {
	objetivo_global = dictionary_create();

	//Recorro los objetivos de los entrenadores para agregarlos al objetivo global
	for(int i = 0; i<list_size(entrenadores); i++) {
		entrenador* entrenador_aux = list_get(entrenadores, i);
		dictionary_iterator(entrenador_aux->pokemons_objetivo, agregar_objetivo_a_objetivo_global);
	}

	//Recorro los adquiridos de los entrenadores para restarlos del objetivo global
	for(int i = 0; i<list_size(entrenadores); i++) {
		entrenador* entrenador_aux = list_get(entrenadores, i);
		dictionary_iterator(entrenador_aux->pokemons_objetivo, restar_adquirido_a_objetivo_global);
	}
}

//Se deberia usar solo en armar_objetivo_global y cuando se captura un pokemon
void agregar_objetivo_a_objetivo_global(char* key, void* value) {

	//Si el pokemon ya existia en el objetivo global, obtengo el valor que tenia y le sumo uno
	if(dictionary_has_key(objetivo_global, key)) {
		dictionary_put(objetivo_global, key, (void*) ((int) dictionary_get(objetivo_global, key) + (int) value));
	}
	//Si el pokemon no existia lo agrego al diccionario con un valor de 1
	else {
		dictionary_put(objetivo_global, key, value);
	}
}

//Resto el pokemon atrapado del objetivo global
void restar_adquirido_a_objetivo_global(char* key, void* value) {
	dictionary_put(objetivo_global, key, (void*) ((int) dictionary_get(objetivo_global, key) - (int) value));
}

posicion* armar_posicion(char* posicion_a_armar) {
	posicion* pos = malloc(sizeof(posicion));

	char** posiciones = string_split(posicion_a_armar,"|");

	pos->posicion_x = atoi(posiciones[0]);
	pos->posicion_y = atoi(posiciones[1]);

	return pos;
}

accion* armar_accion(void(*funcion)(void*), int cpu_requerido){
	accion* nueva_accion = malloc(sizeof(accion));

	nueva_accion->funcion = funcion;
	nueva_accion->cpu_requerido = cpu_requerido;

	return nueva_accion;
}
