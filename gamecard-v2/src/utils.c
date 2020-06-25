#include "utils.h"

void crear_archivo_metadata(int block_size, int blocks){

	char* path_archivo_metadata = devolver_path_archivo_metadata();
	char* block_size_aux = string_itoa(block_size);
	char* blocks_aux = string_itoa(blocks);

	log_info(nuestro_log,"path : %s ", path_archivo_metadata);


	FILE* archivo_metadata  = fopen( path_archivo_metadata , "wb+");
	fclose(archivo_metadata);
	metadata_config = config_create(path_archivo_metadata);

	config_set_value(metadata_config,"BLOCK_SIZE", block_size_aux);
	config_set_value(metadata_config, "BLOCKS", blocks_aux);
	config_set_value(metadata_config, "MAGIC_NUMBER", "TALL_GRASS");

	config_save(metadata_config);
	config_destroy(metadata_config);

}

void crear_archivo_files_metadata(char* nombre_archivo, char* directory,int size, t_list* blocks,char* open){
	char* path_archivo_files_metadata = devolver_path_files_metadata(nombre_archivo);
	char* size_aux = string_itoa(size);
	char* blocks_aux = string_new();
	string_append(&blocks_aux,"[");

	for(int i = 0; i<list_size(blocks);i++){
		string_append(&blocks_aux,string_itoa(list_get(blocks,i)));
		if(i+1 < list_size(blocks)){
			string_append(&blocks_aux,",");
		}
	}
	string_append(&blocks_aux,"]");

	creacion_archivo_files_metadata(path_archivo_files_metadata,directory,size_aux,blocks_aux,open);
}

void creacion_archivo_files_metadata(char* path, char* directory,char* size, char* blocks,char* open){
	string_append(&path, "/Metadata.bin");
	FILE* archivo_files_metadata  = fopen(path, "wb+");

	log_info(nuestro_log,"path a crear : %s ", path);

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

}

void crear_dato(char* numero){
	char* path_dato = devolver_path_dato(numero);

	FILE* archivo_dato  = fopen( path_dato , "w+");
	fclose(archivo_dato);
}

char* obtener_magic_number(){
	metadata_config = config_create (devolver_path_archivo_metadata());
	return config_get_string_value(metadata_config, "MAGIC_NUMBER");

}

char* obtener_open_archivo_metadata_pokemon(char* nombre){
	char* path = devolver_path_files_metadata(nombre);
	string_append(&path,"/Metadata.bin");
	metadata_config = config_create(path);
	return config_get_string_value(metadata_config, "OPEN");
}

int obtener_size_archivo_metadata_pokemon(char* nombre){
	char* path = devolver_path_files_metadata(nombre);
	string_append(&path,"/Metadata.bin");
	metadata_config = config_create(path);
	return config_get_int_value(metadata_config, "SIZE");
}

t_list* obtener_blocks_archivo_metadata_pokemon(char* nombre){
	char* path = devolver_path_files_metadata(nombre);
	string_append(&path,"/Metadata.bin");
	metadata_config = config_create(path);
	char** lista = config_get_array_value(metadata_config, "BLOCKS");
	return crear_t_list(lista);
}

char* obtener_directory_archivo_metadata_pokemon(char* nombre){
	char* path = devolver_path_files_metadata(nombre);
	string_append(&path,"/Metadata.bin");
	metadata_config = config_create(path);
	return config_get_string_value(metadata_config, "DIRECTORY");
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
	metadata_config = config_create (devolver_path_archivo_metadata());
	return config_get_int_value(metadata_config, "BLOCKS");
}

int obtener_block_size(){
	metadata_config = config_create (devolver_path_archivo_metadata());
	return config_get_int_value(metadata_config, "BLOCK_SIZE");
}

char* devolver_path_directorio_files(){
	char* path_directorio_files = string_new();
	string_append(&path_directorio_files, devolver_path_directorio("/Files"));
	return path_directorio_files;
}

char* devolver_path_archivo_metadata(){

	char* path_archivo_metadata = string_new();

	string_append(&path_archivo_metadata, devolver_path_directorio("/Metadata"));
	string_append(&path_archivo_metadata, "/Metadata.bin");


	return path_archivo_metadata;
}

char* devolver_path_files_metadata(char* nombre_archivo){

	char* path_archivo_files = string_new();

	string_append(&path_archivo_files, devolver_path_directorio("/Files"));
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
	string_append(&path_archivo_dato, devolver_path_directorio("/Blocks"));
	string_append(&path_archivo_dato, "/");
	string_append(&path_archivo_dato, numero);
	string_append(&path_archivo_dato, ".bin");

	return path_archivo_dato;
}

char* devolver_path_bitmap(){
	char* path_archivo_bitmap = string_new();
	string_append(&path_archivo_bitmap, devolver_path_directorio("/Metadata"));
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

	return key_a_guardar;
}


//GUARDAR_INFORMACION = NEW POKEMON
void guardar_informacion(char* nombre_pokemon,int posicion_x,int posicion_y,int cantidad){


	//string_append(&informacion_a_guardar,pos_y_aux);
	char* path_nombre_metadata = string_new();
	string_append(&path_nombre_metadata, devolver_path_files_metadata(nombre_pokemon)); //Devolver_path_files_metadata ya verifica si existe o no el pokemon, no hace falta hacer un if
	string_append(&path_nombre_metadata, "/Metadata.bin");
	int pude_abrir_el_archivo = 0;
	while(pude_abrir_el_archivo == 0){
		if(se_puede_abrir_el_archivo(nombre_pokemon)){
			abrir_archivo(nombre_pokemon);
			pude_abrir_el_archivo = 1;
			t_list* bloques = obtener_blocks_archivo_metadata_pokemon(nombre_pokemon);
			t_config* archivo_pokemon;
			int posicion;
			char* key_a_guardar = devolver_posicion_concatenada(posicion_x,posicion_y);
			char* dato_a_escribir = string_new();
			dato_a_escribir = key_a_guardar;
			log_info(nuestro_log,"DATO A ESCRIBIR : %s",dato_a_escribir);

			log_info(nuestro_log,"PATH A ESCRIBIR : %s",path_nombre_metadata);

			if(la_posicion_ya_existe_dentro_del_archivo(posicion_x,posicion_y,nombre_pokemon)){
				log_info(nuestro_log,"ENTRE SI EXISTE UNA POSICION");
				int nueva_cantidad = encontrar_cantidad_en_posicion(posicion_x,posicion_y,nombre_pokemon) + cantidad;
				for(int i = 0; i< list_size(bloques);i++){
					archivo_pokemon = config_create(devolver_path_dato(list_get(bloques,i)));
					config_set_value(archivo_pokemon,key_a_guardar,string_itoa(nueva_cantidad));
					config_save(archivo_pokemon);
					posicion = atoi(list_get(bloques,i));
					//escribir_bloque(path_nombre_metadata,dato_a_escribir);;
					bitarray_set_bit(bitmap,posicion);
					config_destroy(archivo_pokemon);
				}
			}else{
				log_info(nuestro_log,"ENTRE SI NO EXISTE UNA POSICION");
				for(int i = 0; i< list_size(bloques);i++){
					archivo_pokemon = config_create(devolver_path_dato(list_get(bloques,i)));
					config_set_value(archivo_pokemon,key_a_guardar,string_itoa(cantidad));
					config_save(archivo_pokemon);
					posicion = atoi(list_get(bloques,i));
					//escribir_bloque(path_nombre_metadata,dato_a_escribir);;
					bitarray_set_bit(bitmap,posicion);
					config_destroy(archivo_pokemon);
				}
			}
			sleep(leer_tiempo_retardo_operacion());
			cerrar_archivo(nombre_pokemon);
			free(dato_a_escribir);
			list_destroy(bloques);
		}
		else{
			sleep(leer_tiempo_de_reintento_operacion());
		}
	}
	free(path_nombre_metadata);

}

//DISMINUIR_CANTIDAD_DE_POKEMON_EN_LA_POSICION = CATCH POKEMON
void disminuir_cantidad_de_pokemon_en_la_posicion(char* nombre_pokemon,int posicion_x,int posicion_y){

	if(existe_el_pokemon(nombre_pokemon)){
		//string_append(&informacion_a_guardar,pos_y_aux);
		char* path_nombre_metadata = string_new();
		string_append(&path_nombre_metadata, devolver_path_files_metadata(nombre_pokemon));
		string_append(&path_nombre_metadata, "/Metadata.bin");

		int pude_abrir_el_archivo = 0;
		while(pude_abrir_el_archivo == 0){
			if(se_puede_abrir_el_archivo(nombre_pokemon)){
				abrir_archivo(nombre_pokemon);
				pude_abrir_el_archivo = 1;
				t_list* bloques = obtener_blocks_archivo_metadata_pokemon(nombre_pokemon);
				t_config* archivo_pokemon;
				int posicion;
				char* key_a_disminuir = devolver_posicion_concatenada(posicion_x,posicion_y);

				if(la_posicion_ya_existe_dentro_del_archivo(posicion_x,posicion_y,nombre_pokemon)){
					if(encontrar_cantidad_en_posicion(posicion_x,posicion_y,nombre_pokemon)== 1){
						for(int i = 0; i< list_size(bloques);i++){
							archivo_pokemon = config_create(devolver_path_dato(list_get(bloques,i)));
							config_remove_key(archivo_pokemon,key_a_disminuir);
							config_save(archivo_pokemon);
							config_destroy(archivo_pokemon);
						}
					}
					else{
						int nueva_cantidad = encontrar_cantidad_en_posicion(posicion_x,posicion_y,nombre_pokemon) - 1;
						for(int i = 0; i< list_size(bloques);i++){
							archivo_pokemon = config_create(devolver_path_dato(list_get(bloques,i)));
							config_set_value(archivo_pokemon,key_a_disminuir,string_itoa(nueva_cantidad));
							config_save(archivo_pokemon);
							posicion = atoi(list_get(bloques,i));
							//escribir_bloque(path_nombre_metadata,dato_a_escribir);;
							bitarray_set_bit(bitmap,posicion);
							config_destroy(archivo_pokemon);
						}
					}
				}else{
					log_error(logger,"No se encontro pokemons en la posicion");
					log_error(nuestro_log,"No se encontro pokemons en la posicion");
				}
				sleep(leer_tiempo_retardo_operacion());
				cerrar_archivo(nombre_pokemon);
				list_destroy(bloques);
			}
			else{
				sleep(leer_tiempo_de_reintento_operacion());
			}
		}
	}else{
		log_error(logger,"No existe ese pokemon");
		log_error(nuestro_log,"No existe ese pokemon");
	}
}

/*
//DEVOLVER_POSICIONES_POKEMONS = GET POKEMON
char* devolver_posiciones_pokemons(char* nombre_pokemon){

	if(existe_el_pokemon(nombre_pokemon)){
		//string_append(&informacion_a_guardar,pos_y_aux);
		char* path_nombre_metadata = string_new();
		string_append(&path_nombre_metadata, devolver_path_files_metadata(nombre_pokemon));
		string_append(&path_nombre_metadata, "/Metadata.bin");

		int pude_abrir_el_archivo = 0;
		while(pude_abrir_el_archivo == 0){
			if(se_puede_abrir_el_archivo(nombre_pokemon)){
				abrir_archivo(nombre_pokemon);
				pude_abrir_el_archivo = 1;
				t_list* bloques = obtener_blocks_archivo_metadata_pokemon(nombre_pokemon);
				t_config* archivo_pokemon;
				int posicion;
				char* posiciones = string_new();
				string_append(&posiciones,"[");

				for(int i = 0; i< list_size(bloques);i++){
					archivo_pokemon = config_create(devolver_path_dato(list_get(bloques,i)));
					string_append(&posiciones,config_);
					string_append(&posiciones,",");
					config_destroy(archivo_pokemon);
				}
				string_append(&posiciones,"]");
				sleep(leer_tiempo_retardo_operacion());
				cerrar_archivo(nombre_pokemon);
				list_destroy(bloques);
			}
			else{
				sleep(leer_tiempo_de_reintento_operacion());
			}
		}
	}else{
		//Informar mensaje sin posiciones ni cantidades
	}
}
*/

int encontrar_cantidad_en_posicion(int posicion_x,int posicion_y,char* nombre_pokemon){
	t_config* archivo_pokemon;
	char* key_a_buscar = devolver_posicion_concatenada(posicion_x,posicion_y);
	t_list* bloques = obtener_blocks_archivo_metadata_pokemon(nombre_pokemon);

	for(int i = 0; i< list_size(bloques);i++){
		archivo_pokemon = config_create(devolver_path_dato(list_get(bloques,i)));
		if(config_has_property(archivo_pokemon,key_a_buscar)){
			return config_get_int_value(archivo_pokemon,key_a_buscar);
		}
		config_destroy(archivo_pokemon);
	}
	return 0;
}

int existe_el_pokemon(char* nombre_pokemon){
	char* path_archivo_files = string_new();

	string_append(&path_archivo_files, devolver_path_directorio("/Files"));
	string_append(&path_archivo_files,"/");
	string_append(&path_archivo_files,nombre_pokemon);
	return existe_el_directorio(path_archivo_files);

}

void abrir_archivo(char* nombre_pokemon){
	char* path = devolver_path_files_metadata(nombre_pokemon);
	string_append(&path,"/Metadata.bin");

	metadata_config = config_create(path);
	config_set_value(metadata_config,"OPEN","Y");
}

void cerrar_archivo(char* nombre_pokemon){
	char* path = devolver_path_files_metadata(nombre_pokemon);
	string_append(&path,"/Metadata.bin");

	metadata_config = config_create(path);
	config_set_value(metadata_config,"OPEN","N");
}

int se_puede_abrir_el_archivo(char* nombre_pokemon){
	char* open_aux = obtener_open_archivo_metadata_pokemon(nombre_pokemon);
	return string_equals_ignore_case(open_aux,"N");
}

int la_posicion_ya_existe_dentro_del_archivo(int posicion_x,int posicion_y, char* nombre_pokemon){
	t_config* archivo_pokemon;
	char* key_a_buscar = devolver_posicion_concatenada(posicion_x,posicion_y);
	t_list* bloques = obtener_blocks_archivo_metadata_pokemon(nombre_pokemon);

	for(int i = 0; i< list_size(bloques);i++){
		archivo_pokemon = config_create(devolver_path_dato(list_get(bloques,i)));
		if(config_has_property(archivo_pokemon,key_a_buscar)){
			return 1;
		}
		config_destroy(archivo_pokemon);
	}
	return 0;
}

