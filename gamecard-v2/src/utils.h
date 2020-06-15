/*
 * utils.h
 *
 *  Created on: 11 jun. 2020
 *      Author: utnso
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "config-reader.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <commons/config.h>
#include <commons/log.h>
#include<commons/string.h>
#include<commons/bitarray.h>

t_config* metadata_config;
t_config* files_metadata_config;
t_log* logger;
t_log* nuestro_log;
char* punto_montaje_tallgrass;

typedef struct
{
	int posicion_x;
	int posicion_y;
} posicion;

void crear_archivo_metadata(int block_size, int blocks);
void crear_archivo_files_metadata(char* nombre_archivo, char* directory,int size, t_list* blocks,char* open);
void creacion_archivo_files_metadata(char* path, char* directory,char* size, char* blocks,char* open);
void crear_directorio(char* path_directorio);
void crear_dato(int numero);
char* devolver_path_directorio(char* path);
char* devolver_path_archivo_metadata();
char* devolver_path_files_metadata(char* nombre_archivo);
char* devolver_path_dato(char* numero);
char* obtener_magic_number();
int obtener_blocks();
int obtener_block_size();
char* obtener_open_archivo_metadata_pokemon(char* nombre);
int obtener_size_archivo_metadata_pokemon(char* nombre);
t_list* obtener_blocks_archivo_metadata_pokemon(char* nombre);
t_list* crear_t_list(char** array);
char* obtener_directory_archivo_metadata_pokemon(char* nombre);
char* devolver_path_directorio_files();
int existe_el_directorio(char* path_directorio);
void crear_bitmap();

#endif /* UTILS_H_ */
