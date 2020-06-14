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

t_config* metadata_config;
t_log* logger;
t_log* nuestro_log;
char* punto_montaje_tallgrass;


typedef struct
{
	int block_size;
	int blocks;
	char* magic_number;
} dato_metadata;

void crear_archivo_metadata(int block_size, int blocks);
void crear_directorio(char* path_directorio);
char* devolver_path_metadata();
char* devolver_path_archivo_metadata();
char* obtener_magic_number();
int obtener_blocks();
int obtener_block_size();
int existe_el_directorio(char* path_directorio);

#endif /* UTILS_H_ */
