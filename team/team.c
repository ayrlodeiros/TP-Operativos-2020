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

	pthread_create(&conexion_broker,NULL,conectar_broker, NULL);
	pthread_join(conexion_broker,NULL);


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
}

void terminar_team() {
	destruir_config();
	log_destroy(logger);
	log_destroy(nuestro_log);
}
