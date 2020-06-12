#include "utils.h"

void crear_archivo_metadata(int block_size, int blocks){

	char* punto_montaje_tallgrass = leer_punto_montaje_tallgrass();

	if(!existe_el_directorio("Makefile",strcat(punto_montaje_tallgrass,"/Metadata"))){
		log_info(nuestro_log,"ENTRE");
		crear_directorio("/Makefile",strcat(punto_montaje_tallgrass,"/Metadata"));
	}
	char* path_archivo_metadata = strcat(punto_montaje_tallgrass,"/Metadata/Metadata.bin");


	log_info(nuestro_log,"path : %s ", path_archivo_metadata);


	FILE* archivo_metadata  = fopen( path_archivo_metadata , "wb");

	fwrite(strcat("BLOCK_SIZE=",(char*)block_size),sizeof(char),1,archivo_metadata);
	fwrite(strcat("BLOCKS=",(char*)blocks),sizeof(char),1,archivo_metadata);
	fwrite("MAGIC_NUMBER=TALL_GRASS" + blocks,sizeof(char),1,archivo_metadata);

	fclose(archivo_metadata);
}


// CREAR DIRECTORIOS
void crear_directorio(char* nombre_directorio,char* path_directorio){

	mkdir(nombre_directorio, 0700); //Anda
	//crear_metadata(nombre_tabla, criterio, numero_Particiones, tiempo_Compactacion); //Arreglar esto
	//Crear binario con respecto a las particiones y agregar un bloque a cada uno
}

//VERIFICAR SI EXISTE EL DIRECTORIO

int existe_el_directorio(char* nombre_directorio,char* path_directorio){

	DIR* directorio_a_buscar = opendir(path_directorio);
	if(directorio_a_buscar){
		return 1;
	}else {
		log_info(nuestro_log,"El directorio NO existe. Se creara a continuacion");
		crear_directorio(nombre_directorio,path_directorio);

	}
	closedir(directorio_a_buscar);
	return 0;
}


