#include "team-v2.h"


int main(void){

	iniciar_team();
	//
	log_info(nuestro_log, string_from_format("El algoritmo de planificacion es: %d", leer_algoritmo_planificacion()));
	log_info(nuestro_log, string_from_format("La cantidad de entrenadores del equipo es de %d entrenadores", list_size(entrenadores)));
	dictionary_iterator(objetivo_global, mostrar_objetivo_global);


	/*pthread_t* hilo_reintento_conexion_broker;
	pthread_create(&hilo_reintento_conexion_broker,NULL,levantar_conexiones_al_broker, NULL);
	pthread_detach(hilo_reintento_conexion_broker);*/


	realizar_get();

	pthread_t* hilo_planificacion;
	pthread_create(&hilo_planificacion,NULL,(void*) planificar, NULL);
	pthread_detach(hilo_planificacion);

	//AGREGO MANUALMENTE A UN POKEMON PARA PROBAR
	manejar_aparicion_de_pokemon("Pikachu", 15, 0);
	manejar_aparicion_de_pokemon("Pikachu", 105, 0);
	manejar_aparicion_de_pokemon("Charmander", 210, 0);
	//manejar_aparicion_de_pokemon("Pikachu", 510, 15);

	/*for(int i = 0; i < list_size(entrenadores_ready); i++){
		entrenador* entrenador = list_get(entrenadores_ready, i);
		printf("\nPOSICION ENTRENADOR %d: X->%d e Y->%d", i, entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
		printf("\nCANTIDAD MAXIMA POKEMONS ENTRENADOR %d: %d", i, entrenador->cant_maxima_pokemons);
		for(int j = 0; j<list_size(entrenador->pokemons_adquiridos); j++){
			printf("\nPOKEMONS ENTRENADOR %d: %s", i, list_get(entrenador->pokemons_adquiridos, j));
		}
	}*/



	pthread_t* hilo_de_espera;
	pthread_create(&hilo_de_espera,NULL, while_uno, NULL);
	pthread_join(hilo_de_espera, NULL);

	return 0;
}

void while_uno() {
	while(1){

	}
}

void mostrar_objetivo_global(char* key, void* value) {
	log_info(nuestro_log, "Necesito %d pokemons %s", value, key);
}

void iniciar_team() {
	iniciar_config();
	logger = log_create(leer_log_file(), "team", false, LOG_LEVEL_INFO);
	nuestro_log = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2/src/resources/nuestro_log.txt", "team", true, LOG_LEVEL_INFO);
	//Iniciamos las variables globales del constructor
	iniciar_variables_globales();
}

void terminar_team() {
	destruir_config();
	log_destroy(logger);
	log_destroy(nuestro_log);
}


/*printf("\n 1");
	pokemons_sueltos = queue_create();

	pokemon* pokemon_de_prueba  = malloc(sizeof(pokemon));;
	pokemon_de_prueba->nombre = "PEPE";
	pokemon_de_prueba->posicion->posicion_x = 0;
	pokemon_de_prueba->posicion->posicion_y = 0;

	printf("\n 2");
	queue_push(pokemons_sueltos,pokemon_de_prueba);
	//entrenador* entrenador_prueba_1 = list_get(entrenadores,1);
	//entrenador* entrenador_prueba_2 = list_get(entrenadores,0);

	printf("\n 3");
	printf("\n 4");
	entrenador* unentrenador = entrenador_a_ejecutar();
	printf("Posicion X : %d",unentrenador->posicion->posicion_x);
	printf("Posicion Y : %d",unentrenador->posicion->posicion_y);
	printf("\n 5");
	free(pokemon_de_prueba);
	*/

	/*
	for(int i = 0; i < list_size(entrenadores); i++){
		entrenador* entrenador = list_get(entrenadores, i);
		printf("\nPOSICION ENTRENADOR %d: X->%d e Y->%d", i, entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
		printf("\nCANTIDAD MAXIMA POKEMONS ENTRENADOR %d: %d", i, entrenador->cant_maxima_pokemons);
		for(int j = 0; j<list_size(entrenador->pokemons_adquiridos); j++){
			printf("\nPOKEMONS ENTRENADOR %d: %s", i, list_get(entrenador->pokemons_adquiridos, j));
		}
	}
	*/

	//PRUEBA DE CREACION DE OBJETIVO GLOBAL
	/*printf("\nPikachus faltantes: %d\n", (int) dictionary_get(objetivo_global, "Pikachu"));
	printf("\nSquirtle faltantes: %d\n", (int) dictionary_get(objetivo_global, "Squirtle"));
	printf("\nPidgey faltantes: %d\n", (int) dictionary_get(objetivo_global, "Pidgey"));
	printf("\nCharmander faltantes: %d\n", (int) dictionary_get(objetivo_global, "Charmander"));
	printf("\nBulbasaur faltantes: %d\n", (int) dictionary_get(objetivo_global, "Bulbasaur"));
	printf("\nCualquiercosa faltantes: %d\n"), (int) dictionary_get(objetivo_global, "Cualquiercosa");

	printf("\nLo necesito a Charmander? %d\n", el_pokemon_es_requerido("Charmander"));*/
