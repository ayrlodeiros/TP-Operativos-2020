#include "team.h"


//Creacion del hilo


/*void* sumar();
void* restar();
pthread_t entrenadores[3];
*/


int main(void){

	pthread_t* conexion_broker;

	iniciar_team();
	log_info(nuestro_log, leer_algoritmo_planificacion());

	log_info(nuestro_log, string_from_format("La cantidad de entrenadores del equipo es de %d entrenadores", list_size(entrenadores)));


	printf("\n 1");
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

	pthread_create(&conexion_broker,NULL,conectar_broker, NULL);
	pthread_detach(conexion_broker);


	//VER MAS ADELANTE

	t_queue* cola_de_entrenadores = queue_create();
	//queue_push();

	/*pthread_create(&(entrenadores[0]),NULL,sumar,NULL);

	pthread_create(&(entrenadores[1]),NULL,restar,NULL);


	pthread_join(entrenadores[0],NULL);
	pthread_join(entrenadores[1],NULL);

	printf("El valor de i es : %d \n ",i);
	*/

	return 0;
}

void iniciar_team() {
	iniciar_config();
	logger = log_create(leer_log_file(), "team", false, LOG_LEVEL_INFO);
	nuestro_log = log_create("nuestro_log.txt", "team", true, LOG_LEVEL_INFO);
	armar_entrenadores();
}

void terminar_team() {
	destruir_config();
	log_destroy(logger);
	log_destroy(nuestro_log);
}
