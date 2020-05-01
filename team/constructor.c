#include "constructor.h"

//Se deberia ejecutar una sola vez, al iniciar el proceso team
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

pokemon* armar_pokemon(char* nombre, int posicion_x, int posicion_y) {
		pokemons_sueltos = queue_create();

		pokemon* nuevo_pokemon  = malloc(sizeof(pokemon));;
		nuevo_pokemon->nombre = nombre;
		nuevo_pokemon->posicion->posicion_x = posicion_x;
		nuevo_pokemon->posicion->posicion_y = posicion_y;

		queue_push(pokemons_sueltos,nuevo_pokemon);
}

posicion* armar_posicion(char* posicion_a_armar) {
	posicion* pos = malloc(sizeof(posicion));

	char** posiciones = string_split(posicion_a_armar,"|");

	pos->posicion_x = atoi(posiciones[0]);
	pos->posicion_y = atoi(posiciones[1]);

	return pos;
}

//Se deberia ejecutar una sola vez, al iniciar el proceso team
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
