#include "gamecard-v2.h"

int main(void)
{
	iniciar_gamecard();
	t_list* blocks = list_create();

	//list_add(blocks,82);
	//list_add(blocks,21);
	//list_add(blocks,3);
	list_add(blocks,0);


	crear_archivo_files_metadata("Pikachu","N",250,blocks,"N");

	//guardar_informacion("Pikachu",10,50,22);
	guardar_informacion("Pikachu",10,50,1111);
	guardar_informacion("Pikachu",11,51,1111);
	guardar_informacion("Pikachu",12,52,1111);
	guardar_informacion("Pikachu",13,53,1111);
	guardar_informacion("Pikachu",14,54,1111);
	guardar_informacion("Pikachu",15,55,1111);
	guardar_informacion("Pikachu",155,555,1111);
	guardar_informacion("Pikachu",17,57,1111);
	guardar_informacion("Pikachu",1,5,10);
	guardar_informacion("Pikachu",5,3,17);
	guardar_informacion("Pikachu",60,5,10);
	guardar_informacion("Pikachu",1,2,17);
	guardar_informacion("Pikachu",12,3,100);
	guardar_informacion("Pikachu",1,5,17);
	guardar_informacion("Pikachu",4,7,100);
	guardar_informacion("Pikachu",9,12,18);
	guardar_informacion("Pikachu",5,3,17);
	guardar_informacion("Pikachu",6,5,10);
	guardar_informacion("Pikachu",1,2,17);
	guardar_informacion("Pikachu",12,3,10);
	guardar_informacion("Pikachu",10,1,17);
	guardar_informacion("Pikachu",10,2,1);
	guardar_informacion("Pikachu",10,3,17);
	guardar_informacion("Pikachu",10,4,17);
	guardar_informacion("Pikachu",10,5,17);
	guardar_informacion("Pikachu",10,6,17);
	guardar_informacion("Pikachu",55,6,17);
	guardar_informacion("Pikachu",51,6,17);
	guardar_informacion("Pikachu",52,6,17);
	guardar_informacion("Pikachu",1,555,10);
	guardar_informacion("Pikachu",52,65,17);
	guardar_informacion("Pikachu",52,61,17);
	guardar_informacion("Pikachu",52,22,17);
	guardar_informacion("Pikachu",52,13,17);

	char* path_nombre_metadata = string_new();
	string_append(&path_nombre_metadata, devolver_path_files_metadata("Pikachu"));
	string_append(&path_nombre_metadata, "/Metadata.bin");
	t_list* unalista = list_create();
	leer_datos(path_nombre_metadata,unalista);

	for(int i = 0; i< list_size(unalista);i++){
		log_info(nuestro_log,"DATO DE LA LISTA :%s",list_get(unalista,i));
	}


	for(int j = 0;j<10;j++){
		log_info(nuestro_log,"VALOR DEL BITMAP %d: %d",j,bitarray_test_bit(bitmap,j));
	}

	//t_config* archivo_pokemon = config_create(devolver_path_dato(list_get(blocks,0)));
	//log_info(nuestro_log,"1-5 :&d",config_get_int_value(archivo_pokemon,"1-5"));

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
	//log_info(nuestro_log,config_get_string_value(config,"miele"));
	return EXIT_SUCCESS;
}


void iniciar_gamecard() {
	iniciar_config();
	logger = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/logger.txt", "gamecard", false, LOG_LEVEL_INFO);
	nuestro_log = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/nuestro_log.txt", "gamecard", true, LOG_LEVEL_INFO);
	punto_montaje_tallgrass = leer_punto_montaje_tallgrass();
	crear_directorio(punto_montaje_tallgrass);
	crear_archivo_metadata(64,5192);
	creacion_archivo_files_metadata(devolver_path_directorio_files(),"Y","0","","N");

	pthread_mutex_init(&Mutex_Bitmap,NULL);

	//Iniciamos las variables globales del constructor
	flag_bloques_libres = 1; //hay bloques libres
	ultimo_bloque_asignado = 0; //inicio como ultimo bloque asignado el primero

	if(!se_creo_el_bloque()){
		crear_bloque();
	}
	crear_bitmap(); //levanto el bitarray;
}

void terminar_gamecard() {
	destruir_config();
	log_destroy(logger);
	log_destroy(nuestro_log);
}
