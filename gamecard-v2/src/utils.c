#include "utils.h"

void crear_archivo_metadata(int block_size, int blocks){

	char* punto_montaje_tallgrass = leer_punto_montaje_tallgrass();
	char* path_directorio_metadata = strcat(punto_montaje_tallgrass,"/Metadata");
	char* path_archivo_metadata = strcat(path_directorio_metadata,"/Metadata.bin");

	if(!existe_el_directorio(path_directorio_metadata)){
		log_info(nuestro_log,"ENTRE");
		crear_directorio(path_directorio_metadata);
	}


	log_info(nuestro_log,"path : %s ", path_archivo_metadata);


	FILE* archivo_metadata  = fopen( path_archivo_metadata , "wb");
	dato_metadata* dato = malloc(sizeof(dato_metadata));

	dato->block_size = block_size;
	dato->blocks = blocks;
	dato->magic_number = "TALL_GRASS";

	fwrite(&dato,sizeof(dato_metadata),1,archivo_metadata);

	fclose(archivo_metadata);
}

void crear_archivo(char* path_archivo){

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


