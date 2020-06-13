#include "team-v2.h"


int main(void){

	iniciar_team();

	log_info(nuestro_log, string_from_format("El algoritmo de planificacion es: %d", leer_algoritmo_planificacion()));
	log_info(nuestro_log, string_from_format("La cantidad de entrenadores del equipo es de %d entrenadores", list_size(entrenadores)));
	dictionary_iterator(objetivo_global, mostrar_objetivo_global);

	/*pthread_t* hilo_conexion_gameboy;
	pthread_create(&hilo_conexion_gameboy,NULL,levantar_conexiones_al_broker, NULL);
	pthread_detach(hilo_conexion_gameboy);*/

	pthread_t* hilo_reintento_conexion_broker;
	pthread_create(&hilo_reintento_conexion_broker,NULL,levantar_conexiones_al_broker, NULL);
	pthread_detach(hilo_reintento_conexion_broker);

	get_pokemon();

	pthread_t* hilo_planificacion;
	pthread_create(&hilo_planificacion,NULL, planificar, NULL);
	pthread_detach(hilo_planificacion);

	pthread_t* hilo_entrenador_disponible;
	pthread_create(&hilo_entrenador_disponible, NULL, buscar_entrenador_disponible, NULL);
	pthread_detach(hilo_entrenador_disponible);

	//AGREGO MANUALMENTE A UN POKEMON PARA PROBAR
	//manejar_aparicion_de_pokemon("Pikachu", 15, 0);
	//manejar_aparicion_de_pokemon("Squirtle", 5, 0);
	//manejar_aparicion_de_pokemon("Pikachu", 10, 0);
	//manejar_aparicion_de_pokemon("Pikachu", 10, 15);

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
	log_info(nuestro_log,string_from_format("TERMINO EL PROCESO TEAM"));
	destruir_config();
	log_destroy(logger);
	log_destroy(nuestro_log);
}
