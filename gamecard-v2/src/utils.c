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

	log_info(logger, "Se creo el archivo metadata correctamente en el path %s ",path_archivo_metadata);
	free(path_archivo_metadata);
	free(block_size_aux);
	free(blocks_aux);

}

void crear_punto_de_montaje(){
	char** punto_montaje_tallgrass_partido = string_split(punto_montaje_tallgrass,"/");
	char* path = string_new();
	for(int i = 0; i< tamanio_de_lista(punto_montaje_tallgrass_partido) ; i++){
		string_append(&path,"/");
		string_append(&path,punto_montaje_tallgrass_partido[i]);

		crear_directorio(path);
		free(punto_montaje_tallgrass_partido[i]);

	}
    log_info(logger, "se creo el punto de montaje correctamente en el path %s ",punto_montaje_tallgrass);
	free(path);
	free(punto_montaje_tallgrass_partido);

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

	bitarray_set_bit(bitmap,nuevo_bloque_libre); //TODO ver si poner mutex o no
	log_info(logger, "se asigno el bloque [%d] al bitmap",nuevo_bloque_libre);
	log_info(logger, "se asigno el bloque [%d] al files del pokemon : %s",nuevo_bloque_libre,nombre_archivo);
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
	log_info(logger, "Se creo el archivo files metadata correctamente en el path %s ",path);
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
	metadata_config = config_create(path);
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

	if(cantidad > 0){
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
						log_info(logger,"se reemplazo la cantidad en la posicion (%s) con la nueva cantidad (%d)",posicion,cantidad_sumada);
						reescribir_bloques(path_nombre_metadata,dato_a_escribir);

					}else{
						dato_a_escribir = armar_dato_bloque(posicion,cantidad);
						escribir_bloque_v2(path_nombre_metadata,dato_a_escribir);
						log_info(logger,"se agrego la posicion (%s) con la cantidad (%d)",posicion,cantidad);
						free(dato_a_escribir);
					}
					log_info(logger,"Tamaño del archivo del pokemon %s: %d",nombre_pokemon,obtener_size_archivo_metadata_pokemon(nombre_pokemon));
					sleep(leer_tiempo_retardo_operacion());
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
			pthread_mutex_lock(&mutex_modificar_carpeta);
			crear_archivo_files_metadata(nombre_pokemon,"N",250,"N");
			pthread_mutex_unlock(&mutex_modificar_carpeta);
			free(path_nombre_metadata);
			guardar_informacion(nombre_pokemon,posicion_x,posicion_y,cantidad);
		}
	}
	else{
		log_info(nuestro_log,"Cantidad invalida ( cantidad < 0 )");
		log_info(logger,"Cantidad invalida ( cantidad < 0 )");
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

	log_info(logger,"se resetean los bloques del metadata del pokemon : %s",path_nombre_metadata);
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

				sleep(leer_tiempo_retardo_operacion());
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
		return list_create();
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
					log_info(logger,"Se disminuye en 1 la cantidad en la posicion %s,del (Pokemon : %s)",posicion,nombre_pokemon);
					reescribir_bloques(path_nombre_metadata,dato_a_escribir);

					t_list* posiciones_pokemon = posiciones_del_pokemon(path_nombre_metadata);

					cerrar_archivo(nombre_pokemon);
					if(list_is_empty(posiciones_pokemon)){
						pthread_mutex_lock(&mutex_modificar_carpeta);
						char* path_carpeta_pokemon = devolver_path_files_metadata(nombre_pokemon);
						int bloque_a_remover = devolver_ultimo_bloque(path_nombre_metadata);
						liberar_bloque(bloque_a_remover);
						remove(path_nombre_metadata);
						rmdir(path_carpeta_pokemon);
						log_info(logger,"Al no existir mas el pokemon, se borran su carpeta y su metadata (Pokemon : %s)",nombre_pokemon);
						free(path_carpeta_pokemon);
						pthread_mutex_unlock(&mutex_modificar_carpeta);
					}
					list_destroy_and_destroy_elements(posiciones_pokemon,free);
					sleep(leer_tiempo_retardo_operacion());

					list_destroy(bloques);
					free(posicion);
					free(path_nombre_metadata);

					return 1;
				}else{
					log_error(logger,"No se encontro pokemons en la posicion");
					log_error(nuestro_log,"No se encontro pokemons en la posicion");
				}
				sleep(leer_tiempo_retardo_operacion());
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
	pthread_mutex_lock(&mutex_modificar_carpeta);
	char* path_aux = devolver_path_directorio("/Files");
	string_append(&path_archivo_files,path_aux);
	free(path_aux);
	string_append(&path_archivo_files,"/");
	string_append(&path_archivo_files,nombre_pokemon);
	int existe_el_pokemon = existe_el_directorio(path_archivo_files);
	free(path_archivo_files);
	pthread_mutex_unlock(&mutex_modificar_carpeta);
	return existe_el_pokemon;

}

void abrir_archivo(char* nombre_pokemon){
	char* path = devolver_path_files_metadata(nombre_pokemon);
	string_append(&path,"/Metadata.bin");

	metadata_config = config_create(path);
	config_set_value(metadata_config,"OPEN","Y");
	config_destroy(metadata_config);
	log_info(logger,"se abrio el archivo : %s", nombre_pokemon);
	free(path);
}

void cerrar_archivo(char* nombre_pokemon){
	char* path = devolver_path_files_metadata(nombre_pokemon);
	string_append(&path,"/Metadata.bin");

	metadata_config = config_create(path);
	config_set_value(metadata_config,"OPEN","N");
	config_destroy(metadata_config);
	log_info(logger,"se cerro el archivo : %s", nombre_pokemon);
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

//Se mueve del bloques.c
int tamanio_de_lista(char** un_array){
	int i = 0;

	if(!un_array[i]){
		return 0;
	}

	while(un_array[i] != NULL){
		i++;
	}
	return i;
}


//trae todas los inserts de esa url, que es la particion.bin o el .tmp
t_list* leer_datos(char* path_metadata_config){
	FILE *archivo;
	int tamanio_archivo;
	char* lista_de_bloques_string = devolver_lista_de_bloques(path_metadata_config);
	char** lista_de_bloques = string_get_string_as_array(lista_de_bloques_string);
	free(lista_de_bloques_string);
	int size = tamanio_de_lista(lista_de_bloques); // tamano de array de bloques
	char* dato = string_new();
	char* path_bloque; // url de cada block particular
	char* pivot;
	struct stat st;
	for(int i = 0; i<size; i++)
	{
		path_bloque = devolver_path_dato(lista_de_bloques[i]);
		stat(path_bloque,&st);
		tamanio_archivo = st.st_size;

		pivot = malloc(tamanio_archivo+1);
		archivo = fopen(path_bloque,"r");
		fread(pivot,tamanio_archivo,1,archivo);
		fclose(archivo);
		pivot[tamanio_archivo] = '\0';
		if(strcmp(pivot,"&")) //si no es igual a "&" lo agrego a la lista de inserts
			string_append(&dato,pivot);

		free(lista_de_bloques[i]);
		free(pivot);
		free(path_bloque);
	}
	t_list* lista_de_posiciones = list_create();

	agregar_datos_a_la_lista(dato,lista_de_posiciones,path_metadata_config);
	 //parsea el char *inserts por \n y los mete en la lista
	free(dato);
	free(lista_de_bloques);
	return lista_de_posiciones;
}

int cantidad_en_posicion(t_list* lista_de_posiciones,char* posicion_a_buscar){

	int posicion_encontrada = posicion_en_la_lista_de_posiciones_pokemon_a_buscar(lista_de_posiciones,posicion_a_buscar);
	char* aux = string_new();
	char* pivot_aux = list_get(lista_de_posiciones,posicion_encontrada);
	string_append(&aux,pivot_aux);
	char** aux_partido = string_split(aux,"=");
	char* cantidad_encontrada_string = aux_partido[1];
	char* posicion = aux_partido[0];
	int cantidad_encontrada = atoi(cantidad_encontrada_string);
	free(aux);
	free(aux_partido);
	free(cantidad_encontrada_string);
	free(posicion);

	return cantidad_encontrada;


}
