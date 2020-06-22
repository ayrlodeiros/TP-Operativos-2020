#include "gamecard-v2.h"

int main(void)
{
	iniciar_gamecard();

	t_list* blocks = list_create();
	list_add(blocks,40);
	list_add(blocks,21);
	list_add(blocks,82);
	list_add(blocks,3);
	crear_archivo_files_metadata("/Pikachu","N",250,blocks,"Y");

	t_list* lista = obtener_blocks_archivo_metadata_pokemon("/Pikachu");
	for(int i = 0; i< list_size(lista);i++){
		int bloque = atoi(list_get(lista,i));
		escribir_bloque(devolver_path_files_metadata("/Pikachu/Metadata.bin"),bloque);
	}

/*
	log_info(nuestro_log,"tiempo_de_reintento_conexion : %d", leer_tiempo_de_reintento_conexion());
	log_info(nuestro_log,"tiempo_de_reintento_operacion : %d", leer_tiempo_de_reintento_operacion());
	log_info(nuestro_log,"tiempo_retardo_operacion : %d", leer_tiempo_retardo_operacion());
	log_info(nuestro_log,"leer_punto_montaje_tallgrass : %s", leer_punto_montaje_tallgrass());
	log_info(nuestro_log,"ip_broker : %s", leer_ip_broker());
	log_info(nuestro_log,"leer_puerto_broker : %d", leer_puerto_broker());

	log_info(nuestro_log,"BLOCK_SIZE : %d", obtener_block_size());
	log_info(nuestro_log,"BLOCKS : %d", obtener_blocks());
	log_info(nuestro_log,"MAGIC_NUMBER : %s", obtener_magic_number());
*/






	//log_info(nuestro_log,"Directory : %s", obtener_directory_archivo_metadata_pokemon("/Pikachu"));
	//log_info(nuestro_log,"Size : %d", obtener_size_archivo_metadata_pokemon("/Pikachu"));
	//log_info(nuestro_log,"Open : %s", obtener_open_archivo_metadata_pokemon("/Pikachu"));

	log_info(nuestro_log,"Termine");
	return EXIT_SUCCESS;
}


void iniciar_gamecard() {
	iniciar_config();
	logger = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/logger.txt", "gamecard", false, LOG_LEVEL_INFO);
	nuestro_log = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/nuestro_log.txt", "gamecard", true, LOG_LEVEL_INFO);
	crear_directorio("/home/utnso/Escritorio/tall-grass");
	punto_montaje_tallgrass = leer_punto_montaje_tallgrass();
	crear_archivo_metadata(64,5192);
	creacion_archivo_files_metadata(devolver_path_directorio_files(),"Y",0,"","N");

	pthread_mutex_init(&Mutex_Bitmap,NULL);

	//Iniciamos las variables globales del constructor
	flag_bloques_libres = 1; //hay bloques libres
	ultimo_bloque_asignado = 0; //inicio como ultimo bloque asignado el primero

	log_info(nuestro_log,"Prueba 0");

	if(!se_creo_el_bloque()){
		log_info(nuestro_log,"Prueba 1");
		crear_bloque();
		log_info(nuestro_log,"Prueba 2");
	}
	log_info(nuestro_log,"Prueba 3");
	crear_bitmap(); //levanto el bitarray
	log_info(nuestro_log,"Prueba 4");
}

void terminar_gamecard() {
	destruir_config();
	log_destroy(logger);
	log_destroy(nuestro_log);
}
