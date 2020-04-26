#include"utils.h"

void conectar_broker(void) {
	char* ip_broker = leer_ip_broker();
	char* puerto_broker = leer_puerto_broker();

	while(1) {
		log_info(nuestro_log, "Conectando con broker...");
		sleep(leer_tiempo_reconexion());
	}
}

void armar_entrenadores(void) {
	t_list* posiciones = leer_posiciones_entrenadores();
	t_list* pokemons = leer_pokemon_entrenadores();
	t_list* objetivos = leer_objetivos_entrenadores();

	entrenadores = list_create();

	for(int i=0; i< list_size(posiciones);i++){
		list_add(entrenadores, armar_entrenador(list_get(posiciones, i), list_get(pokemons, i), list_get(objetivos, i)));
	}

}

entrenador* armar_entrenador(char* posicion, char* pokemons, char* objetivos){
	entrenador* un_entrenador = malloc(sizeof(entrenador));
	t_list* lista_objetivos = crear_t_list(string_split(objetivos,"|"));

	un_entrenador->posicion = armar_posicion(posicion);
	un_entrenador->pokemons_adquiridos = crear_t_list(string_split(pokemons,"|"));
	un_entrenador->pokemons_objetivo = lista_objetivos;
	un_entrenador->cant_maxima_pokemons = list_size(lista_objetivos);


	return un_entrenador;
}

posicion* armar_posicion(char* posicion_a_armar) {
	posicion* pos = malloc(sizeof(posicion));

	char** posiciones = string_split(posicion_a_armar,"|");

	pos->posicion_x = atoi(posiciones[0]);
	pos->posicion_y = atoi(posiciones[1]);

	return pos;
}

size_t obtener_tamanio_entrenador(char* posicion, char* pokemons, char* objetivos){
	return sizeof(char)* (string_length(posicion) + string_length(pokemons) + string_length(objetivos)) + sizeof(int);
}
