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

void appeared_pokemon(char* nombre_pokemon,int posicion_x,int posicion_y){
	if(el_pokemon_es_requerido(nombre_pokemon)){
		armar_pokemon(nombre_pokemon,posicion_x,posicion_y);
	}
}

t_list* get_pokemon(char* nombre_pokemon){

}

//Se deberia usar solo en armar_objetivo_global
void agregar_objetivo_a_objetivo_global(char* pokemon_objetivo) {

	//Si el pokemon ya existia en el objetivo global, obtengo el valor que tenia y le sumo uno
	if(esta_en_el_objetivo_global(pokemon_objetivo)) {
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
	//Consigo los entrenador que estan en ready
	t_list* entrenadores_a_ejecutar = list_filter(entrenadores, (int*) el_entrenador_se_puede_planificar);

	//Ordeno los entrenadores en funcion de que tan cerca estan del pokemon
	list_sort(entrenadores_a_ejecutar,(int*)el_entrenador1_esta_mas_cerca);



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
	return esta_en_el_objetivo_global(nombre_pokemon) && necesito_mas_de_ese_pokemon(nombre_pokemon);
}

int esta_en_el_objetivo_global(char* nombre_pokemon){
	if(dictionary_has_key(objetivo_global,nombre_pokemon)){ //Lo pongo con un IF, porque dictionary_has_key devuelve un bool,y no se si rompe todo el bool.
		return 1;
	}
	else{
		return 0;
	}
}

//Si encuentra en el objetivo global, una cantidad mayor a 0 de ese pokemon, entonces lo necesitamos. Al devolver un numero mayor a 0 -> True
int necesito_mas_de_ese_pokemon(char* nombre_pokemon){
	return dictionary_get(objetivo_global,nombre_pokemon);
}


