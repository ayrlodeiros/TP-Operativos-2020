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

t_log* logger;
t_log* nuestro_log;


typedef struct
{
	int block_size;
	int blocks;
	char* magic_number;
} dato_metadata;

void crear_archivo_metadata(int block_size, int blocks);
void crear_directorio(char* path_directorio);
int existe_el_directorio(char* path_directorio);

#endif /* UTILS_H_ */
