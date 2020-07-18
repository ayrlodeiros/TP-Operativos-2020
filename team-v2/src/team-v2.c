#include "team-v2.h"


int main(void){

	iniciar_team();
<<<<<<< HEAD
	log_info(nuestro_log, string_from_format("El algoritmo de planificacion es: %d", leer_algoritmo_planificacion()));
	log_info(nuestro_log, string_from_format("La cantidad de entrenadores del equipo es de %d entrenadores", list_size(entrenadores)));
	dictionary_iterator(objetivo_global, mostrar_objetivo_global);

	//intentar_conectar_al_broker();

	//AGREGO MANUALMENTE A UN POKEMON PARA PROBAR

	pthread_t* hilo_planificacion;
	pthread_create(&hilo_planificacion,NULL,(void*) planificar, NULL);
	pthread_detach(hilo_planificacion);

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
=======

	log_info(nuestro_log, "La cantidad de entrenadores del equipo es de %d entrenadores", list_size(entrenadores));
	dictionary_iterator(objetivo_global, mostrar_objetivo_global);

	pthread_t* hilo_conexion_gameboy;
	pthread_create(&hilo_conexion_gameboy, NULL, atender_conexion_gameboy, NULL);
	pthread_detach(hilo_conexion_gameboy);

	pthread_t* hilo_reintento_conexion_broker;
	pthread_create(&hilo_reintento_conexion_broker, NULL, levantar_conexiones_al_broker, NULL);
	pthread_detach(hilo_reintento_conexion_broker);

	get_pokemon();

	pthread_t* hilo_entrenador_disponible;
	pthread_create(&hilo_entrenador_disponible, NULL, buscar_entrenador_disponible, NULL);
	pthread_detach(hilo_entrenador_disponible);

	pthread_t* hilo_planificacion;
	pthread_create(&hilo_planificacion, NULL, planificar, NULL);
	pthread_join(hilo_planificacion, NULL);
>>>>>>> develop

	return 0;
}

<<<<<<< HEAD
void while_uno() {
	while(1){

	}
}

=======
>>>>>>> develop
void mostrar_objetivo_global(char* key, void* value) {
	log_info(nuestro_log, "Necesito %d pokemons %s", value, key);
}

void iniciar_team() {
	iniciar_config();
	logger = log_create(leer_log_file(), "team", false, LOG_LEVEL_INFO);
	nuestro_log = log_create(leer_nuestro_log_file(), "team", true, LOG_LEVEL_INFO);
	//Iniciamos las variables globales del constructor
	iniciar_variables_globales();
<<<<<<< HEAD

	pthread_mutex_lock(&lock_de_planificacion);
}

void terminar_team() {
	destruir_config();
	log_destroy(logger);
	log_destroy(nuestro_log);
=======
>>>>>>> develop
}
