/*
 * serializar.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef SERIALIZAR_H_
#define SERIALIZAR_H_

#include<stdio.h>
#include<stdlib.h>

typedef struct {

} t_paquete;

void* serializar_paquete(t_paquete* paquete, int *bytes);

#endif /* SERIALIZAR_H_ */
