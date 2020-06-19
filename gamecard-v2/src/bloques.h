/*
 * bloques.h
 *
 *  Created on: 18 jun. 2020
 *      Author: utnso
 */

#ifndef BLOQUES_H_
#define BLOQUES_H_

#include "utils.h"

void modificar_tamanio_bloque(char* path_bloque,int tamanio);
char* devolver_lista_de_bloques(char* path_particion);

#endif /* BLOQUES_H_ */
