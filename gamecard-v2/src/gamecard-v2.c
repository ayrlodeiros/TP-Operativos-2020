#include "gamecard-v2.h"

int main(void)
{
	iniciar_gamecard();


	log_info(nuestro_log,"tiempo_de_reintento_conexion : %d", leer_tiempo_de_reintento_conexion());
	log_info(nuestro_log,"tiempo_de_reintento_operacion : %d", leer_tiempo_de_reintento_operacion());
	log_info(nuestro_log,"tiempo_retardo_operacion : %d", leer_tiempo_retardo_operacion());
	log_info(nuestro_log,"leer_punto_montaje_tallgrass : %s", leer_punto_montaje_tallgrass());
	log_info(nuestro_log,"ip_broker : %s", leer_ip_broker());
	log_info(nuestro_log,"leer_puerto_broker : %d", leer_puerto_broker());

	log_info(nuestro_log,"BLOCK_SIZE : %d", obtener_block_size());
	log_info(nuestro_log,"BLOCKS : %d", obtener_blocks());
	log_info(nuestro_log,"MAGIC_NUMBER : %s", obtener_magic_number());


	return EXIT_SUCCESS;
}


void iniciar_gamecard() {
	iniciar_config();
	logger = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/logger.txt", "gamecard", false, LOG_LEVEL_INFO);
	nuestro_log = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/nuestro_log.txt", "gamecard", true, LOG_LEVEL_INFO);
	crear_directorio("/home/utnso/Escritorio/tall-grass");
	punto_montaje_tallgrass = leer_punto_montaje_tallgrass();
	crear_archivo_metadata(64,5192);
	//Iniciamos las variables globales del constructor
}

void terminar_gamecard() {
	destruir_config();
	log_destroy(logger);
	log_destroy(nuestro_log);
}
