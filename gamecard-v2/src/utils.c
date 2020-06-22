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

void crear_dato(int numero){
	char* numero_aux = string_itoa(numero);
	char* path_dato = devolver_path_dato(numero_aux);

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
	string_append(&path_archivo_files,nombre_archivo);
	if(!existe_el_directorio(path_archivo_files)){
		crear_directorio(path_archivo_files);
	}
	log_info(nuestro_log,"path files metadata: %s ", path_archivo_files);

	return path_archivo_files;
}

char* devolver_path_directorio(char* path){

	char* path_directorio = string_new();

	string_append(&path_directorio, punto_montaje_tallgrass);
	string_append(&path_directorio, path);

	if(!existe_el_directorio(path_directorio)){
		crear_directorio(path_directorio);
	}else{
		log_info(nuestro_log,"Existe el directorio : %s", path_directorio);
	}

	return path_directorio;
}

char* devolver_path_dato(char* numero){
	char* path_archivo_dato = string_new();
	char* numero_a_string = string_itoa(numero);
	string_append(&path_archivo_dato, devolver_path_directorio("/Blocks"));
	string_append(&path_archivo_dato, "/");
	string_append(&path_archivo_dato, numero_a_string);
	string_append(&path_archivo_dato, ".bin");

	log_info(nuestro_log,"path datos metadata: %s ", path_archivo_dato);

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
		log_info(nuestro_log,"Creo el directorio : %s",path_directorio);
		mkdir(path_directorio, 0700); //IMPORTANTE : Al MKdir hay que pasarle el path, NO el nombre del directorio
	}else{
		log_info(nuestro_log,"El directorio YA existe.");
	}
	 //Anda
	//crear_metadata(nombre_tabla, criterio, numero_Particiones, tiempo_Compactacion); //Arreglar esto
	//Crear binario con respecto a las particiones y agregar un bloque a cada uno
}

//VERIFICAR SI EXISTE EL DIRECTORIO

int existe_el_directorio(char* path_directorio){

	log_info(nuestro_log,"PATH DIRECTORIO a buscar : %s",path_directorio);
	DIR* directorio_a_buscar = opendir(path_directorio);
	if(directorio_a_buscar){
		return 1;
	}else {
		log_info(nuestro_log,"El directorio NO existe.");
	}
	closedir(directorio_a_buscar);
	return 0;
}


