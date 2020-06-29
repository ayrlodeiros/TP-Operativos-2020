/*
 * bloques.h
 *
 *  Created on: 18 jun. 2020
 *      Author: utnso
 */

#ifndef BLOQUES_H_
#define BLOQUES_H_

#include "utils.h"

//void escribir_bloque(char* path_config,char* dato);

void agregar_bloques_al_metadata(char* path_nombre_metadata,int tamanio_dato,int tamanio_disponible_del_ultimo_bloque);
void agregar_dato_al_bloque (char* path_nombre_metadata,char* dato_a_escribir);
void escribir_bloque_v2(char* path_nombre_metadata,char* dato_a_escribir);
void leer_datos(char* path_metadata_config, t_list* lista_de_datos);
int se_encontro_la_posicion(char* path_metadata_config, char* posicion_a_buscar);
void agregar_datos_a_la_lista(char *datos, t_list* lista);
void escribir_bloque_asignado(int bloque);
void limpiar_bloque(int bloque);
int se_creo_el_bloque();
void crear_bloque();
void modificar_tamanio_bloque(char* path_bloque,int tamanio);
void actualizar_tamanio_bloque(char* path_bloque);
void guardar_en_bloque(char* path_bloque, char* dato);
int tamanio_libre_real(int bloque);
int tamanio_libre_del_bloque(int bloque);
void agregar_bloque(char* path_bloque);
void modificar_bloque(char* path_particion, char* lista_bloques);
int obtener_primer_bloque_libre(char* path_bloque);
int el_bloque_esta_lleno(int bloque);
int el_bloque_esta_vacio(int bloque);
int tamanio_libre_del_ultimo_bloque(char* path);
int devolver_ultimo_bloque(char* path);
int tamanio_de_lista(char** un_array);
char* devolver_lista_de_bloques(char* path_particion);
void empezar_file_metadata(char* path_archivo_files_metadata,char* es_directorio,int bloque, int tamanio, char* esta_abierto);
void cargar_datos_del_file_metadata (char* path_pokemon);
void asignar_tamanio_y_bloque (char* path,int tamanio);

#endif /* BLOQUES_H_ */
