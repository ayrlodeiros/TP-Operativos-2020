/*
 * team.h
 *
 *  Created on: 20 abr. 2020
 *      Author: utnso
 */


typedef struct nodo{
    pthread_t entrenador;
    struct nodo *ptr;
}nodo;

typedef struct cola{
    nodo*inicio;
}cola;

agregar_entrenador_al_final_de_la_cola(int dato, cola *micola);
llenar_nodo(nodo *un_nodo, cola *unacola);
la_cola_esta_vacia(cola *una_cola);
hay_un_nodo_siguiente(nodo* un_nodo);
obtener_primer_entrenador(pthread_t un_entrenador, cola *una_cola);

#ifndef TEAM_H_
#define TEAM_H_


#include "utils.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>




#endif /* TEAM_H_ */
