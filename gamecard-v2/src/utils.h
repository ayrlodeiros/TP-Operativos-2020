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
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <semaphore.h>
#include"commons/txt.h"
#include <commons/collections/dictionary.h>

t_config* metadata_config;
t_config* files_metadata_config;
t_log* logger;
t_log* nuestro_log;
char* punto_montaje_tallgrass;

t_bitarray* bitmap;
int bitmap_file_descriptor;
int flag_bloques_libres;
int ultimo_bloque_asignado;
pthread_mutex_t Mutex_Bitmap;
pthread_mutex_t asignar_bloque;
pthread_mutex_t mutex_liberar_bloque;
pthread_mutex_t mutex_facu;

typedef struct
{
	int posicion_x;
	int posicion_y;
} posicion;

void crear_archivo_metadata(int block_size, int blocks);
void crear_archivo_files_metadata(char* nombre_archivo, char* directory,int size,char* open);
void creacion_archivo_files_metadata(char* path, char* directory,char* size, char* blocks,char* open);
void crear_directorio(char* path_directorio);
void crear_dato(char* numero);
char* devolver_path_directorio(char* path);
char* devolver_path_archivo_metadata();
char* devolver_path_files_metadata(char* nombre_archivo);
char* devolver_path_dato(char* numero);
char* devolver_path_bitmap();
char* obtener_magic_number();
t_list* armar_mensaje_get(char* nombre_pokemon);
int obtener_blocks();
int obtener_block_size();
char* obtener_open_archivo_metadata_pokemon(char* nombre);
int obtener_size_archivo_metadata_pokemon(char* nombre);
t_list* obtener_blocks_archivo_metadata_pokemon(char* nombre);
t_list* crear_t_list(char** array);
char* obtener_directory_archivo_metadata_pokemon(char* nombre);
char* devolver_path_directorio_files();
int existe_el_directorio(char* path_directorio);
int existe_el_pokemon(char* nombre_pokemon);
char* armar_dato_bloque(char* posicion,int cantidad);
int se_puede_abrir_el_archivo(char* nombre_pokemon);
int la_posicion_ya_existe_dentro_del_archivo(char* posicion_a_buscar,char* nombre_pokemon);
void crear_lista_posiciones(char* key, int value);
t_list* posiciones_del_pokemon(char* path_nombre_metadata);
void abrir_archivo(char* nombre_pokemon);
void cerrar_archivo(char* nombre_pokemon);
void resetear_bloques_metadata_pokemon(char* path_nombre_metadata);

#endif /* UTILS_H_ */
