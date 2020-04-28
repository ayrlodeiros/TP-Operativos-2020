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


	//entrenador_a_ejecutar();

	for(int i = 0; i < list_size(entrenadores); i++){
		entrenador* entrenador = list_get(entrenadores, i);
		printf("\nPOSICION ENTRENADOR %d: X->%d e Y->%d", i, entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
		printf("\nCANTIDAD MAXIMA POKEMONS ENTRENADOR %d: %d", i, entrenador->cant_maxima_pokemons);
		for(int j = 0; j<list_size(entrenador->pokemons_adquiridos); j++){
			printf("\nPOKEMONS ENTRENADOR %d: %s", i, list_get(entrenador->pokemons_adquiridos, j));
		}
	}

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
