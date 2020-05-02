#include"utils.h"

void conectar_broker(void) {
	char* ip_broker = leer_ip_broker();
	char* puerto_broker = leer_puerto_broker();
	int tiempo_reconexion = leer_tiempo_reconexion();

	while(1) {
		log_info(nuestro_log, "Conectando con broker...");
		log_info(logger, "10. Inicio de proceso de reintento de comunicación con el Broker.");
		sleep(tiempo_reconexion);
	}
}

void appeared_pokemon(char* nombre_pokemon,int posicion_x,int posicion_y){
	if(el_pokemon_es_requerido(nombre_pokemon)){
		armar_pokemon(nombre_pokemon,posicion_x,posicion_y);
	}
}

t_list* get_pokemon(char* nombre_pokemon){

}

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado){
	entrenador->estado = un_estado;
}

int el_entrenador_se_puede_planificar(entrenador* un_entrenador){
	return un_entrenador->estado == READY;
}

entrenador* entrenador_a_ejecutar(){
	//Consigo los entrenador que estan en ready
	t_list* entrenadores_a_ejecutar = list_filter(entrenadores, el_entrenador_se_puede_planificar);

	//Ordeno los entrenadores en funcion de que tan cerca estan del pokemon
	list_sort(entrenadores_a_ejecutar, el_entrenador1_esta_mas_cerca);

	for(int i = 0; i < list_size(entrenadores_a_ejecutar); i++){
			entrenador* entrenador = list_get(entrenadores_a_ejecutar, i);
			printf("\nPOSICION ENTRENADOR %d: X->%d e Y->%d", i, entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
			printf("\nCANTIDAD MAXIMA POKEMONS ENTRENADOR %d: %d", i, entrenador->cant_maxima_pokemons);
			for(int j = 0; j<list_size(entrenador->pokemons_adquiridos); j++){
				printf("\nPOKEMONS ENTRENADOR %d: %s", i, list_get(entrenador->pokemons_adquiridos, j));
			}
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

void restar_cpu_disponible(entrenador* entrenador, int cantidad) {
	entrenador->cpu_disponible -= cantidad;
}

void sumar_cpu_usado(entrenador* entrenador, int cantidad) {
	entrenador->cpu_usado += cantidad;
}



int el_pokemon_es_requerido(char* nombre_pokemon){
	return dictionary_has_key(objetivo_global,nombre_pokemon)
			&& necesito_mas_de_ese_pokemon(nombre_pokemon);
}

//Hablarla
/*int esta_en_el_objetivo_global(char* nombre_pokemon){
	//Lo pongo con un IF, porque dictionary_has_key devuelve un bool,y no se si rompe por el bool.
	if(dictionary_has_key(objetivo_global,nombre_pokemon)){
		return 1;
	}
	else{
		return 0;
	}
}*/

//Si encuentra en el objetivo global, una cantidad mayor a 0 de ese pokemon, entonces lo necesitamos. Al devolver un numero mayor a 0 -> True
int necesito_mas_de_ese_pokemon(char* nombre_pokemon){
	return ((int) dictionary_get(objetivo_global,nombre_pokemon)) > 0;
}


