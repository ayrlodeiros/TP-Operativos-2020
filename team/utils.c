#include"utils.h"

void conectar_broker(void) {
	char* ip_broker = leer_ip_broker();
	char* puerto_broker = leer_puerto_broker();

	while(1) {
		log_info(nuestro_log, "Conectando con broker...");
		log_info(logger, "10. Inicio de proceso de reintento de comunicación con el Broker.");
		sleep(leer_tiempo_reconexion());
	}
}

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

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado){
	entrenador->estado = un_estado;
}

int el_entrenador_se_puede_planificar(entrenador* un_entrenador){
	return un_entrenador->estado == READY;
}

entrenador* entrenador_a_ejecutar(){
	t_list* entrenadores_listos = list_filter(entrenadores, (int*) el_entrenador_se_puede_planificar);

	//Ver si se puede usar list_sort
	t_list* entrenadores_ordenados = list_sorted(entrenadores_listos,(int*)el_entrenador1_esta_mas_cerca);


	for(int i = 0; i < list_size(entrenadores_ordenados); i++){
			entrenador* entrenador = list_get(entrenadores_ordenados, i);
			printf("\nPOSICION ENTRENADOR %d: X->%d e Y->%d", i, entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
			printf("\nCANTIDAD MAXIMA POKEMONS ENTRENADOR %d: %d", i, entrenador->cant_maxima_pokemons);
			for(int j = 0; j<list_size(entrenador->pokemons_adquiridos); j++){
				printf("\nPOKEMONS ENTRENADOR %d: %s", i, list_get(entrenador->pokemons_adquiridos, j));
			}
		}

	//VER LO DEL DEADLOCK, ACA?
	if(hay_deadlock(list_get(entrenadores_ordenados,0),list_get(entrenadores_ordenados,1))){

		entrenador* entrenador1 = list_get(entrenadores_ordenados,0);
		entrenador* entrenador2 = list_get(entrenadores_ordenados,1);

		cambiar_estado_entrenador(entrenador1,READY); //Dice que bloquiemos a los 2 , y despues pasemos uno a READY, asi que directamente el entrenador1 lo dejo ready
		cambiar_estado_entrenador(entrenador2,BLOCK);

		//VER DE SACAR
		//mover_entrenador_a_la_posicion(entrenador1,entrenador2->posicion->posicion_x,entrenador2->posicion->posicion_y);

		return NULL;
	}else{
		return list_get(entrenadores_ordenados,0);
	}



}


int distancia_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon) {
	return (int) (fabs(entrenador->posicion->posicion_x - pokemon->posicion->posicion_x)) + (int) (fabs(entrenador->posicion->posicion_y - pokemon->posicion->posicion_y));
}

int el_entrenador1_esta_mas_cerca(entrenador* entrenador1, entrenador* entrenador2) {
	int distancia_al_pokemon_entrenador1 = distancia_del_entrenador_al_pokemon(entrenador1,queue_peek(pokemons_sueltos));
	int distancia_al_pokemon_entrenador2 = distancia_del_entrenador_al_pokemon(entrenador2,queue_peek(pokemons_sueltos));

	if(distancia_al_pokemon_entrenador1 <= distancia_al_pokemon_entrenador2){
		printf("\n El resultado de la funcion es: 1");
		return 1;
	}
	else {
		printf("\n El resultado de la funcion es: 0");
		return 0;
	}

}

int hay_deadlock(entrenador* entrenador1, entrenador* entrenador2){
	return distancia_del_entrenador_al_pokemon(entrenador1,queue_peek(pokemons_sueltos)) == distancia_del_entrenador_al_pokemon(entrenador2,queue_peek(pokemons_sueltos));
}

void restar_cpu_disponible(entrenador* entrenador, int cantidad) {
	entrenador->cpu_disponible -= cantidad;
}

void sumar_cpu_usado(entrenador* entrenador, int cantidad) {
	entrenador->cpu_usado += cantidad;
}
