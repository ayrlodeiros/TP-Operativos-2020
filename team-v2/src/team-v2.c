#include "team-v2.h"


int main(void){

	pthread_t* conexion_broker;

	iniciar_team();
	log_info(nuestro_log, string_from_format("El algoritmo de planificacion es: %d", leer_algoritmo_planificacion()));

	log_info(nuestro_log, string_from_format("La cantidad de entrenadores del equipo es de %d entrenadores", list_size(entrenadores)));

	pthread_create(&conexion_broker,NULL,conectar_broker, NULL);
	//NO LO ESPERO
	pthread_detach(conexion_broker);
	//LO ESPERO
	//pthread_join(conexion_broker, NULL);

	while(1) {
		log_info(nuestro_log, string_from_format("VALOR DEL SOCKET: %d", socket_broker));
		if(funciona_la_conexion_con_broker()){
			log_info(nuestro_log, "FUNCIONA");
		} else {
			log_info(nuestro_log, "NO FUNCIONA");
		}
		sleep(1);
	}


	return 0;
}

void iniciar_team() {
	//Leemos el config
	iniciar_config();
	//Levantamos los archivos de logeo
	logger = log_create(leer_log_file(), "team", false, LOG_LEVEL_INFO);
	nuestro_log = log_create("src/resources/nuestro_log.txt", "team", true, LOG_LEVEL_INFO);
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
