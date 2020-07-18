#include "gamecard-v2.h"

int main(void)
{
<<<<<<< HEAD
	iniciar_gamecard();
	t_list* blocks = list_create();

	//list_add(blocks,82);
	//list_add(blocks,21);
	//list_add(blocks,3);
	list_add(blocks,0);
	crear_archivo_files_metadata("Pikachu","N",250,blocks,"N");

	guardar_informacion("Pikachu",10,50,22);
	guardar_informacion("Pikachu",10,50,1111);
=======

	//DATO = POSICION_X-POSICION_Y=CANTIDAD
	//char* obtener_posicion_del_dato(char* dato_a_escribir) -> POSICION_X-POSICION_Y
	//int obtener_cantidad_del_dato(char* dato_a_escribir) -> CANTIDAD

	//disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",10,2); -> CATCH
	//NEW => void guardar_informacion(char* nombre_pokemon,int posicion_x,int posicion_y,int cantidad)
	//GET => t_list* armar_mensaje_get(char* nombre_pokemon) => Devuelve una lista de DATOS

	/*LISTA DE FREES :
	 *
	 * IMPORTANTE ALAN => Cuando uses la lista de armar_mensaje_get, acordate de destruirla despues de usarla, asi evitamos memory leaks.
	 *
	 * BLOQUES-C
	 *
	 * 1) Agregue 1 free en bitmap.c linea 17 y anda
	 * 2) Agregue 1 free en bitmap.c linea 24 y anda
	 * 3) Bloques.c linea 187, puse 1 free y anda [free(path_bloque_a_escribir);]
	 * 4) Bloques.c linea 192, puse 1 free y anda [free(dato_a_escribir_restante);]
	 * 5) Bloques.c linea 186, puse 1 free y anda [free(a_escribir);]
	 * 6) Bloques.c linea 195, puse 1 free y anda [free(a_escribir_restante);]
	 * 7) Bloques.c linea 195, puse 1 free y anda [free(string_bloque_a_escribir);]
	 * 8) Bloques.c linea 335, puse 1 free y anda [free(pivot_partido);]
	 * 9) Bloques.c linea 172, puse 1 free y anda [free(string_bloque_a_escribir);]
	 * 10) Bloques.c linea 518, puse 1 free y anda [free(string_i);]
	 * 11) Bloques.c linea 133, puse 1 free y anda [free(string_dato);]
	 * 12) Bloques.c linea 147, puse 1 free y anda [free(posicion);]
	 * 13) Bloques.c linea 252, puse 1 free y anda [free(cantidad);]
	 * 14) Bloques.c linea 352, puse 1 free y anda [free(string_bloque);]
	 * 15) Bloques.c linea 473, puse 1 free y anda [free(bloque_aux);]
	 * 16) Bloques.c linea 325, puse 1 free y anda [free(lista_de_bloques);]
	 * 17) Bloques.c linea 325, puse 1 list_destroy y anda [list_destroy(bloques);]
	 * 18) Bloques.c linea 325, puse 1 list_destroy y anda [list_destroy(lista_de_posiciones);]
	 * 19) Bloques.c cantidad_en_posicion, puse 1 free y anda [free(cantidad_encontrada_string);]
	 * 20) Bloques.c se_encuentra_la_posicion_en_la_lista_de_posiciones_pokemons, puse 1 free y anda [free(posicion);]
	 * 21) Bloques.c se_encuentra_la_posicion_en_la_lista_de_posiciones_pokemons, puse 1 free y anda [free(aux_partido);]
	 * 22) Bloques.c se_encuentra_la_posicion_en_la_lista_de_posiciones_pokemons, puse 1 free y anda [free(aux);]
	 * 23) Bloques.c se_encuentra_la_posicion_en_la_lista_de_posiciones_pokemons, puse 1 free y anda [free(posicion);]
	 * 24) Bloques.c posicion_en_la_lista_de_la_posicion_a_buscar, puse 1 free y anda [free(posicion);]
	 * 25) Bloques.c posicion_en_la_lista_de_la_posicion_a_buscar, puse 1 free y anda [free(posicion);]
	 * 26) Bloques.c posicion_en_la_lista_de_posiciones_pokemon_a_buscar, puse 1 free y anda [free(posicion);]
	 * 27) Bloques.c posicion_en_la_lista_de_posiciones_pokemon_a_buscar, puse 1 free y anda [free(posicion);]
	 * 28) Bloques.c agregar_datos_a_la_lista, puse 1 free y anda [free(posicion);]
	 * 29) Bloques.c agregar_datos_a_la_lista, puse 1 free y anda [free(cantidad_string);]
	 * 30) Bloques.c borrar_posicion, puse 1 free y anda [free(posicion_nueva);]
	 * 31) Bloques.c reescribir_bloques, puse 1 free y anda [free(lista_de_bloques[i]);]
	 * 32) Bloques.c devolver_ultimo_bloque, puse 1 free y anda [free(lista_bloques[i]);]
	 * 33) Bloques.c escribir_bloque_v2, puse 1 free y anda [free(lista_de_bloques[i]);]
	 * 34) Bloques.c se_encuentra_la_posicion_en_la_lista_de_posiciones_pokemons, puse 2 free y anda [free(aux_partido[1]);]
	 * 35) Bloques.c obtener_cantidad_del_dato, puse 1 free y anda [free(aux_partido[0]);]
	 * 36) Bloques.c reescribir_bloques, puse 1 list_destroy_and_destroy_elements(lista_de_posiciones,free);
	 * 37) Bloques.c escribir_bloque_sin_asignar, cambie un string_new por "". char* a_escribir = "";
	 * 38) Bloques.c escribir_bloque_v2, cambie un string_new por "". char* a_escribir = "";
	 * 39) Bloques.c obtener_cantidad_del_dato, 1 free(aux_partido[1]);
	 * 40) Bloques.c obtener_posicion_x_del_dato, 1 free(posicion_partida[0]);
	 * 41) Bloques.c obtener_posicion_x_del_dato, 1 free(posicion_partida[1]);
	 * 42) Bloques.c obtener_posicion_y_del_dato, 1 free(posicion_partida[1]);
	 * 42) Bloques.c obtener_posicion_y_del_dato, 1 free(posicion_partida[0]);
	 *
	 *
	 * UTILS.C
	 *
	 * 1) Agregue en utils,c (linea 247) 2 frees y anda bien
	 * 2) Utils.c [obtener_blocks_archivo_metadata_pokemon] 1 free [free(lista);]
	 * 3) Utils.c [resetear_bloques_metadata_pokemon] 1 free [free(string_nuevo_bloque);]
	 * 4) Utils.c [armar_dato_bloque] 1 free [free(string_cantidad);]
	 * 5) Agregue 1 config_Destroy en abrir y otro en cerrar archivo.
	 * 6) Utils.c [devolver_path_directorio_files] 1 free [free(path_aux);]
	 * 7) Utils.c [devolver_path_archivo_metadata] 1 free [free(path_aux);]
	 * 8) Utils.c [devolver_path_files_metadata] 1 free [free(path_aux);]
	 * 9) Utils.c [devolver_path_dato] 1 free [free(path_aux);]
	 * 10) Utils.c [devolver_path_bitmap] 1 free [free(path_aux);]
	 * 11) Utils.c [guardar_informacion] 2 free [free(path_aux);]
	 * 12) Utils.c [armar_mensaje_get] 1 free [free(path_aux);]
	 * 13) Utils.c [disminuir_cantidad_de_pokemon_en_la_posicion] 1 free [free(path_aux);]
	 * 14) Utils.c [crear_archivo_files_metadata] 1 free [free(string_nuevo_bloque_libre);]
	 * 15) Utils.c [guardar_informacion] 1 list_destroy_and_destroy_elements(lista_de_posiciones,free);
	 * 16) Utils.c [disminuir_cantidad_de_pokemon_en_la_posicion] 1 list_destroy_and_destroy_elements(lista_de_datos,free);
	 * 17) Utils.c [la_posicion_ya_existe_dentro_del_archivo] 1 list_destroy_and_destroy_elements(lista_de_posiciones,free);
	 * 18) Utils.c [posiciones_del_pokemon] 1 list_destroy_and_destroy_elements(posiciones_pokemon,free);
	 * 19) Utils.c [guardar_informacion] movi 1 [free(dato_a_escribir)];
	 * 20) Utils.c [reescribir_bloques] 1 list_destroy_and_destroy_elements(lista_de_posiciones,free);
	 * 21) Utils.c [disminuir_cantidad_de_pokemon_en_la_posicion] 1 free(posicion);
	 * 22) Utils.c [guardar_informacion] 1 free(posicion);
	 * 23) Utils.c [existe_el_pokemon] 1 free(path_archivo_files);
	 * 24) Utils.c [disminuir_cantidad_de_pokemon_en_la_posicion] 1 free(path_nombre_metadata);
	 * 25) Utils.c [existe_el_pokemon] 1 free(path_aux);
	 * 26) Utils.c [la_posicion_ya_existe_dentro_del_archivo] 1 free(path_aux);
	 *
	 * SEMAFOROS:
	 * liberar_bloque -> Se agrego pthread_mutex_lock(&Mutex_Bitmap);
	 *
	 *
	 */


	iniciar_gamecard();

	/*
	guardar_informacion("Pikachu",10,2,100);
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",10,2);

	guardar_informacion("Pikachu",105,50,226);
	guardar_informacion("Squirtle",105,50,10);
	guardar_informacion("Pikachu",105,50,1111);
>>>>>>> develop
	guardar_informacion("Pikachu",11,51,1111);
	guardar_informacion("Pikachu",12,52,1111);
	guardar_informacion("Pikachu",13,53,1111);
	guardar_informacion("Pikachu",14,54,1111);
	guardar_informacion("Pikachu",15,55,1111);
	guardar_informacion("Pikachu",155,555,1111);
	guardar_informacion("Pikachu",17,57,1111);
<<<<<<< HEAD
	guardar_informacion("Pikachu",1,5,10);
=======
	guardar_informacion("Pikachu",19,5,10);
>>>>>>> develop
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
<<<<<<< HEAD
	guardar_informacion("Pikachu",10,1,17);
=======
	guardar_informacion("Pikachu",10,1,17); // ACA ROMPE -> LA POSICION 12-3 = 10 la guarda bien
>>>>>>> develop
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
<<<<<<< HEAD
	log_info(nuestro_log,"ULTIMO GUARDAR");
	guardar_informacion("Pikachu",1,5,1000);
	//guardar_informacion("Pikachu",512,621,17);
	guardar_informacion("Pikachu",100,12,18);




=======
	guardar_informacion("Pikachu",1,5,1000);
	guardar_informacion("Pikachu",512,621,17);
	guardar_informacion("Pikachu",100,12,18);

	guardar_informacion("Squirtle",1,1,10);
	guardar_informacion("Squirtle",2,2,20);
	guardar_informacion("Squirtle",3,3,30);
	guardar_informacion("Squirtle",5,5,50);


	t_list* posiciones = armar_mensaje_get("Pikachu");
	if(!list_is_empty(posiciones)){
		for(int i = 0; i<list_size(posiciones);i++ ){
			log_info(nuestro_log,"Posicion %d : %s",i,list_get(posiciones,i));
		}
	}list_destroy_and_destroy_elements(posiciones,free);

	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",10,2);
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",10,2);
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",100,202);
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",105,50);
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",105,50);
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",105,50);

	posiciones = armar_mensaje_get("Pikachu");
	if(!list_is_empty(posiciones)){
		for(int i = 0; i<list_size(posiciones);i++ ){
			log_info(nuestro_log,"Posicion %d : %s",i,list_get(posiciones,i));
		}
	}
	list_destroy_and_destroy_elements(posiciones,free);

	//log_info(nuestro_log,"PRUEBA 1");

	//disminuir_cantidad_de_pokemon_en_la_posicion("PokemonInexistente",105,50);

	posiciones = armar_mensaje_get("Squirtle");
	if(!list_is_empty(posiciones)){
		for(int i = 0; i<list_size(posiciones);i++ ){
			log_info(nuestro_log,"Posicion %d : %s",i,list_get(posiciones,i));
		}
	}
	list_destroy_and_destroy_elements(posiciones,free);
>>>>>>> develop

	//buscar_posicion_en_el_archivo(path_nombre_metadata,"52-13");

	for(int j = 0;j<10;j++){
		log_info(nuestro_log,"VALOR DEL BITMAP %d: %d",j,bitarray_test_bit(bitmap,j));
	}
<<<<<<< HEAD

/*char* path_nombre_metadata = string_new();
		string_append(&path_nombre_metadata, devolver_path_files_metadata("Pikachu"));
		string_append(&path_nombre_metadata, "/Metadata.bin");
	void mostrar_datos(char* key, int valor){
			log_info(nuestro_log,"Valor Key: %s",key);
			log_info(nuestro_log,"Valor Cantidad: %d",valor);
	}
	dictionary_iterator(leer_datos(path_nombre_metadata),mostrar_datos);
	*/


	/*
	for(int i = 0; i < list_size(lista_de_posiciones);i++){
		log_info(nuestro_log,"Valor Posiciones : %s", list_get(lista_de_posiciones,i));
	}
*/


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
=======
*/



	//TODO BORRAR
	pthread_t* hilo_infinito;
	pthread_create(&hilo_infinito,NULL, while_infinito, NULL);
	pthread_join(hilo_infinito, NULL);
>>>>>>> develop

	log_info(nuestro_log,"Termine");
	//log_info(nuestro_log,config_get_string_value(config,"miele"));
	terminar_gamecard();
	return EXIT_SUCCESS;
}

<<<<<<< HEAD

void iniciar_gamecard() {
	iniciar_config();
	logger = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/logger.txt", "gamecard", false, LOG_LEVEL_INFO);
	nuestro_log = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/nuestro_log.txt", "gamecard", true, LOG_LEVEL_INFO);
	punto_montaje_tallgrass = leer_punto_montaje_tallgrass();
	crear_directorio(punto_montaje_tallgrass);
	crear_archivo_metadata(64,5192);
	creacion_archivo_files_metadata(devolver_path_directorio_files(),"Y","0","","N");

	pthread_mutex_init(&Mutex_Bitmap,NULL);
=======
void while_infinito() {
	while(1);
}
void iniciar_gamecard() {
	iniciar_config();
	logger = log_create(leer_path_logger(), "gamecard", false, LOG_LEVEL_INFO);
	nuestro_log = log_create(leer_path_nuestro_logger(), "gamecard", true, LOG_LEVEL_INFO);
	punto_montaje_tallgrass = leer_punto_montaje_tallgrass();
	crear_punto_de_montaje();
	//crear_archivo_metadata(64,1000);
	creacion_archivo_files_metadata(devolver_path_directorio_files(),"Y","0","","N");

	pthread_mutex_init(&Mutex_Bitmap,NULL);
	pthread_mutex_init(&asignar_bloque,NULL);
	pthread_mutex_init(&mutex_liberar_bloque,NULL);
	pthread_mutex_init(&mutex_modificar_bloque,NULL);
	pthread_mutex_init(&mutex_modificar_carpeta,NULL);
>>>>>>> develop

	//Iniciamos las variables globales del constructor
	flag_bloques_libres = 1; //hay bloques libres
	ultimo_bloque_asignado = 0; //inicio como ultimo bloque asignado el primero

	if(!se_creo_el_bloque()){
		crear_bloque();
	}
	crear_bitmap(); //levanto el bitarray;

<<<<<<< HEAD
=======
	levantar_conexiones();
>>>>>>> develop
}

void terminar_gamecard() {
	destruir_config();
	bitarray_destroy(bitmap);
	log_destroy(logger);
	log_destroy(nuestro_log);
}
