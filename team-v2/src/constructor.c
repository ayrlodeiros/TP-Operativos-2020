#include "constructor.h"

//Se deberia ejecutar una sola vez en el team
void iniciar_variables_globales() {
	armar_entrenadores();
	armar_objetivo_global();
	pokemons_sueltos = queue_create();
	entrenadores_ready = list_create();
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
	t_list* lista_pokemons_objetivo = crear_t_list(string_split(objetivos,"|"));
	t_list* lista_pokemons_adquiridos = crear_t_list(string_split(pokemons,"|"));

	un_entrenador->posicion = armar_posicion(posicion);
	un_entrenador->estado = NEW;
	un_entrenador->pokemons_adquiridos = lista_pokemons_adquiridos;
	un_entrenador->pokemons_objetivo = lista_pokemons_objetivo;
	un_entrenador->cant_maxima_pokemons = list_size(lista_pokemons_objetivo);

	return un_entrenador;
}

//Se deberia ejecutar una sola vez, en el metodo inciar_variables_globales
void armar_objetivo_global() {
	objetivo_global = dictionary_create();

	//Recorro los objetivos de los entrenadores para agregarlos al objetivo global
	for(int i = 0; i<list_size(entrenadores); i++) {
		entrenador* entrenador_aux = list_get(entrenadores, i);
		for(int j = 0; j<list_size(entrenador_aux->pokemons_objetivo); j++) {
			agregar_objetivo_a_objetivo_global(list_get(entrenador_aux->pokemons_objetivo, j));
		}
	}

	for(int i = 0; i<list_size(entrenadores); i++) {
		entrenador* entrenador_aux = list_get(entrenadores, i);
		for(int j = 0; j<list_size(entrenador_aux->pokemons_adquiridos); j++) {
			restar_adquirido_a_objetivo_global(list_get(entrenador_aux->pokemons_adquiridos, j));
		}
	}
}

//Se deberia usar solo en armar_objetivo_global
void agregar_objetivo_a_objetivo_global(char* pokemon_objetivo) {

	//Si el pokemon ya existia en el objetivo global, obtengo el valor que tenia y le sumo uno
	if(dictionary_has_key(objetivo_global, pokemon_objetivo)) {
		dictionary_put(objetivo_global, pokemon_objetivo, dictionary_get(objetivo_global, pokemon_objetivo)+1);
	}
	//Si el pokemon no existia lo agrego al diccionario con un valor de 1
	else {
		dictionary_put(objetivo_global, pokemon_objetivo, 1);
	}
}

//Resto el pokemon atrapado del objetivo global
void restar_adquirido_a_objetivo_global(char* pokemon_adquirido) {
	dictionary_put(objetivo_global, pokemon_adquirido, dictionary_get(objetivo_global, pokemon_adquirido)-1);

}


//Arma un pokemon con los parametros pasados y lo agrega a la cola de pokemons sueltos
void armar_pokemon(char* nombre, int posicion_x, int posicion_y) {
		pokemon* nuevo_pokemon  = malloc(sizeof(pokemon));;
		nuevo_pokemon->nombre = nombre;
		nuevo_pokemon->posicion = armar_posicion(string_from_format("%d|%d", posicion_x, posicion_y));

		queue_push(pokemons_sueltos,nuevo_pokemon);
}

posicion* armar_posicion(char* posicion_a_armar) {
	posicion* pos = malloc(sizeof(posicion));

	char** posiciones = string_split(posicion_a_armar,"|");

	pos->posicion_x = atoi(posiciones[0]);
	pos->posicion_y = atoi(posiciones[1]);

	return pos;
}

accion* armar_accion(void(*closure)(entrenador*),int cpu_requerido){
	accion* accion = malloc(sizeof(accion));

	accion->closure = closure;
	accion->cpu_requerido = cpu_requerido;

	return accion;
}
