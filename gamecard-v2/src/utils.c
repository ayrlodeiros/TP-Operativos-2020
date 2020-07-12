#include "utils.h"

void crear_archivo_metadata(int block_size, int blocks){

	char* path_archivo_metadata = devolver_path_archivo_metadata();
	char* block_size_aux = string_itoa(block_size);
	char* blocks_aux = string_itoa(blocks);

	FILE* archivo_metadata  = fopen( path_archivo_metadata , "wb+");
	fclose(archivo_metadata);
	metadata_config = config_create(path_archivo_metadata);

	config_set_value(metadata_config,"BLOCK_SIZE", block_size_aux);
	config_set_value(metadata_config, "BLOCKS", blocks_aux);
	config_set_value(metadata_config, "MAGIC_NUMBER", "TALL_GRASS");

	config_save(metadata_config);
	config_destroy(metadata_config);
	free(path_archivo_metadata);
	free(block_size_aux);
	free(blocks_aux);

}

void crear_archivo_files_metadata(char* nombre_archivo, char* directory,int size,char* open){
	pthread_mutex_lock(&mutex_modificar_bloque);
	char* path_archivo_files_metadata = devolver_path_files_metadata(nombre_archivo);
	char* size_aux = string_itoa(size);
	char* blocks_aux = string_new();

	string_append(&blocks_aux,"[");
	int nuevo_bloque_libre = obtener_nuevo_bloque_libre();
	char* string_nuevo_bloque_libre = string_itoa(nuevo_bloque_libre);
	string_append(&blocks_aux,string_nuevo_bloque_libre);
	free(string_nuevo_bloque_libre);

	bitarray_set_bit(bitmap,nuevo_bloque_libre);

	string_append(&blocks_aux,"]");


	creacion_archivo_files_metadata(path_archivo_files_metadata,directory,size_aux,blocks_aux,open);
	free(size_aux);
	free(blocks_aux);

	pthread_mutex_unlock(&mutex_modificar_bloque);
}

void creacion_archivo_files_metadata(char* path, char* directory,char* size, char* blocks,char* open){
	string_append(&path, "/Metadata.bin");
	FILE* archivo_files_metadata  = fopen(path, "wb+");

	fclose(archivo_files_metadata);
	files_metadata_config = config_create(path);
	config_set_value(files_metadata_config,"DIRECTORY", directory);
	if(string_equals_ignore_case(directory,"N")){
		config_set_value(files_metadata_config, "SIZE", size);
		config_set_value(files_metadata_config, "BLOCKS", blocks);
		config_set_value(files_metadata_config, "OPEN", open);
	}

	config_save(files_metadata_config);
	config_destroy(files_metadata_config);

	free(path);

}

void crear_dato(char* numero){
	char* path_dato = devolver_path_dato(numero);

	FILE* archivo_dato  = fopen( path_dato , "w+");
	fclose(archivo_dato);
	free(path_dato);
}

char* obtener_magic_number(){
	char* path = devolver_path_archivo_metadata();
	metadata_config = config_create (path);
	char* magic_number = config_get_string_value(metadata_config, "MAGIC_NUMBER");
	config_destroy(metadata_config);
	free(path);
	return magic_number;

}

char* obtener_open_archivo_metadata_pokemon(char* nombre){
	char* path = devolver_path_files_metadata(nombre);
	string_append(&path,"/Metadata.bin");
	metadata_config = config_create(path);
	char* open = string_new();
	string_append(&open,config_get_string_value(metadata_config, "OPEN"));
	free(path);
	config_destroy(metadata_config);
	return open;
}

int obtener_size_archivo_metadata_pokemon(char* nombre){
	char* path = devolver_path_files_metadata(nombre);
	string_append(&path,"/Metadata.bin");
	metadata_config = config_create(path);
	int size = config_get_int_value(metadata_config, "SIZE");
	free(path);
	config_destroy(metadata_config);
	return size;
}

t_list* obtener_blocks_archivo_metadata_pokemon(char* nombre){
	char* path = devolver_path_files_metadata(nombre);
	string_append(&path,"/Metadata.bin");
	metadata_config = config_create(path);
	char** lista = config_get_array_value(metadata_config, "BLOCKS");
	free(path);
	config_destroy(metadata_config);
	t_list* nueva_lista = crear_t_list(lista);
	for(int i = 0; i < tamanio_de_lista(lista); i++){
		free(lista[i]);
	}
	free(lista);
	return nueva_lista;
}

char* obtener_directory_archivo_metadata_pokemon(char* nombre){
	char* path = devolver_path_files_metadata(nombre);
	string_append(&path,"/Metadata.bin");
	metadata_config = config_create(path);
	char* directory = string_new();
	string_append(&directory,config_get_string_value(metadata_config, "DIRECTORY"));
	free(path);
	config_destroy(metadata_config);
	return directory;
}

t_list* crear_t_list(char** array) {
	t_list* lista = list_create();
	int pos = 0;
	while(array[pos] != NULL) {
		list_add(lista, array[pos]);
		pos++;
	}
	return lista;
}

int obtener_blocks(){
	char* path = devolver_path_archivo_metadata();
	metadata_config = config_create (path);
	int blocks = config_get_int_value(metadata_config, "BLOCKS");
	free(path);
	config_destroy(metadata_config);
	return blocks;
}

int obtener_block_size(){
	char* path = devolver_path_archivo_metadata();
	metadata_config = config_create (path);
	int block_size = config_get_int_value(metadata_config, "BLOCK_SIZE");
	free(path);
	config_destroy(metadata_config);
	return block_size;
}

char* devolver_path_directorio_files(){
	char* path_directorio_files = string_new();
	char* path_aux = devolver_path_directorio("/Files");
	string_append(&path_directorio_files,path_aux);
	free(path_aux);
	return path_directorio_files;
}

char* devolver_path_archivo_metadata(){

	char* path_archivo_metadata = string_new();

	char* path_aux = devolver_path_directorio("/Metadata");
	string_append(&path_archivo_metadata,path_aux);
	free(path_aux);
	string_append(&path_archivo_metadata, "/Metadata.bin");


	return path_archivo_metadata;
}

char* devolver_path_files_metadata(char* nombre_archivo){

	char* path_archivo_files = string_new();

	char* path_aux = devolver_path_directorio("/Files");
	string_append(&path_archivo_files,path_aux);
	free(path_aux);
	string_append(&path_archivo_files,"/");
	string_append(&path_archivo_files,nombre_archivo);
	if(!existe_el_directorio(path_archivo_files)){
		crear_directorio(path_archivo_files);
	}
	return path_archivo_files;
}

char* devolver_path_directorio(char* path){

	char* path_directorio = string_new();

	string_append(&path_directorio, punto_montaje_tallgrass);
	string_append(&path_directorio, path);

	if(!existe_el_directorio(path_directorio)){
		crear_directorio(path_directorio);
	}
	return path_directorio;
}

char* devolver_path_dato(char* numero){
	char* path_archivo_dato = string_new();
	char* path_aux = devolver_path_directorio("/Blocks");
	string_append(&path_archivo_dato,path_aux);
	free(path_aux);
	string_append(&path_archivo_dato, "/");
	string_append(&path_archivo_dato, numero);
	string_append(&path_archivo_dato, ".bin");

	return path_archivo_dato;
}

char* devolver_path_bitmap(){
	char* path_archivo_bitmap = string_new();
	char* path_aux = devolver_path_directorio("/Metadata");
	string_append(&path_archivo_bitmap,path_aux);
	free(path_aux);
	string_append(&path_archivo_bitmap,"/Bitmap.bin");

	return path_archivo_bitmap;
}

// CREAR DIRECTORIOS
void crear_directorio(char* path_directorio){

	if(!existe_el_directorio(path_directorio)){
		mkdir(path_directorio, 0700); //IMPORTANTE : Al MKdir hay que pasarle el path, NO el nombre del directorio
	}
}

//VERIFICAR SI EXISTE EL DIRECTORIO

int existe_el_directorio(char* path_directorio){

	DIR* directorio_a_buscar = opendir(path_directorio);
	if(directorio_a_buscar){
		closedir(directorio_a_buscar);
		return 1;
	}
	closedir(directorio_a_buscar);
	return 0;
}

char* devolver_posicion_concatenada (int posicion_x,int posicion_y){
	char* key_a_guardar = string_new();

	char* pos_x_aux = string_itoa(posicion_x);
	char* pos_y_aux = string_itoa(posicion_y);
	string_append(&key_a_guardar,pos_x_aux);
	string_append(&key_a_guardar,"-");
	string_append(&key_a_guardar,pos_y_aux);
	//Agregue en utils,c (linea 247) 2 frees y anda bien
	free(pos_x_aux);
	free(pos_y_aux);

	return key_a_guardar;
}


//GUARDAR_INFORMACION = NEW POKEMON
void guardar_informacion(char* nombre_pokemon,int posicion_x,int posicion_y,int cantidad){

	//string_append(&informacion_a_guardar,pos_y_aux);
	if(existe_el_pokemon(nombre_pokemon)){
		char* path_nombre_metadata = string_new();
		char* path_aux = devolver_path_files_metadata(nombre_pokemon);
		string_append(&path_nombre_metadata,path_aux);
		free(path_aux);
		string_append(&path_nombre_metadata, "/Metadata.bin");
		int pude_abrir_el_archivo = 0;

		while(pude_abrir_el_archivo == 0){
			if(se_puede_abrir_el_archivo(nombre_pokemon)){
				abrir_archivo(nombre_pokemon);
				pude_abrir_el_archivo = 1;
				t_list* bloques = obtener_blocks_archivo_metadata_pokemon(nombre_pokemon);

				char* posicion = devolver_posicion_concatenada(posicion_x,posicion_y);
				char* dato_a_escribir;
				if(la_posicion_ya_existe_dentro_del_archivo(posicion,nombre_pokemon)){
					t_list* lista_de_posiciones = leer_datos(path_nombre_metadata);
					int cantidad_sumada = cantidad + cantidad_en_posicion(lista_de_posiciones,posicion);
					dato_a_escribir = armar_dato_bloque(posicion,cantidad_sumada);
					list_destroy_and_destroy_elements(lista_de_posiciones,free);
					reescribir_bloques(path_nombre_metadata,dato_a_escribir);

				}else{
					dato_a_escribir = armar_dato_bloque(posicion,cantidad);
					escribir_bloque_v2(path_nombre_metadata,dato_a_escribir);
					free(dato_a_escribir);
				}
				//sleep(leer_tiempo_retardo_operacion());
				free(posicion);
				cerrar_archivo(nombre_pokemon);
				list_destroy(bloques);
				free(path_nombre_metadata);
			}
			else{
				sleep(leer_tiempo_de_reintento_operacion());
			}
		}
	}else{
		char* path_nombre_metadata = string_new();

		char* path_aux = devolver_path_files_metadata(nombre_pokemon);
		string_append(&path_nombre_metadata, path_aux);
		free(path_aux);
		string_append(&path_nombre_metadata, "/Metadata.bin");

		crear_archivo_files_metadata(nombre_pokemon,"N",250,"N");
		free(path_nombre_metadata);
		guardar_informacion(nombre_pokemon,posicion_x,posicion_y,cantidad);
	}
}

void resetear_bloques_metadata_pokemon(char* path_nombre_metadata){
	t_config* config = config_create(path_nombre_metadata);
	int nuevo_bloque = obtener_nuevo_bloque_libre();
	bitarray_set_bit(bitmap,nuevo_bloque);
	char* bloque_vacio = string_new();
	string_append(&bloque_vacio,"[");
	char* string_nuevo_bloque = string_itoa(nuevo_bloque);
	string_append(&bloque_vacio,string_nuevo_bloque);
	string_append(&bloque_vacio,"]");

	config_set_value(config,"BLOCKS",bloque_vacio);
	free(bloque_vacio);
	free(string_nuevo_bloque);
	config_save(config);
	config_destroy(config);
}

t_list* armar_mensaje_get(char* nombre_pokemon){

	int pude_abrir_el_archivo = 0;
	t_list* lista_de_posiciones_get;
	if(existe_el_pokemon(nombre_pokemon)){
		char* path_nombre_metadata = string_new();
		char* path_aux = devolver_path_files_metadata(nombre_pokemon);
		string_append(&path_nombre_metadata,path_aux);
		free(path_aux);
		string_append(&path_nombre_metadata, "/Metadata.bin");
		while(pude_abrir_el_archivo == 0){

			if(se_puede_abrir_el_archivo(nombre_pokemon)){
				abrir_archivo(nombre_pokemon);
				pude_abrir_el_archivo = 1;

				lista_de_posiciones_get = leer_datos(path_nombre_metadata);

				//sleep(leer_tiempo_retardo_operacion());
				cerrar_archivo(nombre_pokemon);
				free(path_nombre_metadata);
				return lista_de_posiciones_get;
			}
			else{
				sleep(leer_tiempo_de_reintento_operacion());
			}
		}
	}
	else{
		log_info(logger,"No se encontro al Pokemon");
		log_info(nuestro_log,"No se encontro al Pokemon");
		return lista_de_posiciones_get;
	}

}

char* armar_dato_bloque(char* posicion,int cantidad){
	char* dato = string_new();
	string_append(&dato,posicion);
	string_append(&dato,"=");
	char* string_cantidad = string_itoa(cantidad);
	string_append(&dato,string_cantidad);
	free(string_cantidad);
	string_append(&dato,"\n");

	return dato;
}

//DISMINUIR_CANTIDAD_DE_POKEMON_EN_LA_POSICION = CATCH POKEMON
int disminuir_cantidad_de_pokemon_en_la_posicion(char* nombre_pokemon,int posicion_x,int posicion_y){

	if(existe_el_pokemon(nombre_pokemon)){
		//string_append(&informacion_a_guardar,pos_y_aux);
		char* path_nombre_metadata = string_new();
		char* path_aux = devolver_path_files_metadata(nombre_pokemon);
		string_append(&path_nombre_metadata, path_aux);
		free(path_aux);
		string_append(&path_nombre_metadata,"/Metadata.bin");

		int pude_abrir_el_archivo = 0;
		while(pude_abrir_el_archivo == 0){
			if(se_puede_abrir_el_archivo(nombre_pokemon)){
				abrir_archivo(nombre_pokemon);
				pude_abrir_el_archivo = 1;
				t_list* bloques = obtener_blocks_archivo_metadata_pokemon(nombre_pokemon);
				char* posicion = devolver_posicion_concatenada(posicion_x,posicion_y);
				t_list* lista_de_datos = leer_datos(path_nombre_metadata);
				char* dato_a_escribir;

				if(la_posicion_ya_existe_dentro_del_archivo(posicion,nombre_pokemon)){
					int cantidad_en_pos = cantidad_en_posicion(lista_de_datos,posicion);
					int nueva_cantidad_en_posicion = cantidad_en_pos - 1;
					dato_a_escribir = armar_dato_bloque(posicion,nueva_cantidad_en_posicion);
					list_destroy_and_destroy_elements(lista_de_datos,free);
					reescribir_bloques(path_nombre_metadata,dato_a_escribir);

					//sleep(leer_tiempo_retardo_operacion());
					cerrar_archivo(nombre_pokemon);
					list_destroy(bloques);
					free(posicion);
					free(path_nombre_metadata);

					return 1;
				}else{
					log_error(logger,"No se encontro pokemons en la posicion");
					log_error(nuestro_log,"No se encontro pokemons en la posicion");
				}
				//sleep(leer_tiempo_retardo_operacion());
				cerrar_archivo(nombre_pokemon);
				list_destroy(bloques);
				free(posicion);
				list_destroy_and_destroy_elements(lista_de_datos,free);
				free(path_nombre_metadata);
			}
			else{
				sleep(leer_tiempo_de_reintento_operacion());
			}
		}
	}else{
		log_error(logger,"No existe ese pokemon");
		log_error(nuestro_log,"No existe ese pokemon");
	}
	return 0;
}


int existe_el_pokemon(char* nombre_pokemon){
	char* path_archivo_files = string_new();

	char* path_aux = devolver_path_directorio("/Files");
	string_append(&path_archivo_files,path_aux);
	free(path_aux);
	string_append(&path_archivo_files,"/");
	string_append(&path_archivo_files,nombre_pokemon);
	int existe_el_pokemon = existe_el_directorio(path_archivo_files);
	free(path_archivo_files);
	return existe_el_pokemon;

}

void abrir_archivo(char* nombre_pokemon){
	char* path = devolver_path_files_metadata(nombre_pokemon);
	string_append(&path,"/Metadata.bin");

	metadata_config = config_create(path);
	config_set_value(metadata_config,"OPEN","Y");
	config_destroy(metadata_config);
	free(path);
}

void cerrar_archivo(char* nombre_pokemon){
	char* path = devolver_path_files_metadata(nombre_pokemon);
	string_append(&path,"/Metadata.bin");

	metadata_config = config_create(path);
	config_set_value(metadata_config,"OPEN","N");
	config_destroy(metadata_config);
	free(path);
}

int se_puede_abrir_el_archivo(char* nombre_pokemon){
	char* open_aux = obtener_open_archivo_metadata_pokemon(nombre_pokemon);
	int se_puede_abrir = string_equals_ignore_case(open_aux,"N");
	free(open_aux);
	return se_puede_abrir;
}

int la_posicion_ya_existe_dentro_del_archivo(char* posicion_a_buscar,char* nombre_pokemon){
	char* path_nombre_metadata = string_new();
	char* path_aux = devolver_path_files_metadata(nombre_pokemon);
	string_append(&path_nombre_metadata,path_aux);
	free(path_aux);
	string_append(&path_nombre_metadata, "/Metadata.bin");
	t_list* lista_de_posiciones = posiciones_del_pokemon(path_nombre_metadata);
	free(path_nombre_metadata);
	int se_encuentra_la_posicion = se_encuentra_la_posicion_en_la_lista(lista_de_posiciones,posicion_a_buscar);
	list_destroy_and_destroy_elements(lista_de_posiciones,free);
	return se_encuentra_la_posicion;
}

t_list* posiciones_del_pokemon(char* path_nombre_metadata){
	t_list* posiciones_pokemon = leer_datos(path_nombre_metadata);
	t_list* posiciones = list_create();

	for(int i = 0 ;i< list_size(posiciones_pokemon);i++){
		char* posicion = obtener_posicion_del_dato(list_get(posiciones_pokemon,i));
		list_add(posiciones,posicion);
	}
	list_destroy_and_destroy_elements(posiciones_pokemon,free);
	return posiciones;
}
