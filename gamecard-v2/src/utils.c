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

char* obtener_magic_number(){
	metadata_config = config_create (devolver_path_archivo_metadata());
	return config_get_string_value(metadata_config, "MAGIC_NUMBER");

}

int obtener_blocks(){
	metadata_config = config_create (devolver_path_archivo_metadata());
	return config_get_int_value(metadata_config, "BLOCKS");
}

int obtener_block_size(){
	metadata_config = config_create (devolver_path_archivo_metadata());
	return config_get_int_value(metadata_config, "BLOCK_SIZE");
}


char* devolver_path_archivo_metadata(){

	char* path_archivo_metadata = string_new();

	string_append(&path_archivo_metadata, devolver_path_metadata());
	string_append(&path_archivo_metadata, "/Metadata.bin");


	return path_archivo_metadata;
}

char* devolver_path_metadata(){

	char* path_directorio_metadata = string_new();

	string_append(&path_directorio_metadata, punto_montaje_tallgrass);
	string_append(&path_directorio_metadata, "/Metadata");

	if(!existe_el_directorio(path_directorio_metadata)){
		log_info(nuestro_log,"ENTRE");
		crear_directorio(path_directorio_metadata);
	}

	return path_directorio_metadata;
}

// CREAR DIRECTORIOS
void crear_directorio(char* path_directorio){

	if(!existe_el_directorio(path_directorio)){
		log_info(nuestro_log,"Creo el directorio : %s.",path_directorio);
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

	log_info(nuestro_log,"PATH DIRECTORIO a buscar : %s.",path_directorio);
	DIR* directorio_a_buscar = opendir(path_directorio);
	if(directorio_a_buscar){
		return 1;
	}else {
		log_info(nuestro_log,"El directorio NO existe.");
	}
	closedir(directorio_a_buscar);
	return 0;
}


