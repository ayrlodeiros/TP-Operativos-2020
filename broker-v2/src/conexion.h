/*
 * conexion.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef CONEXION_H_
#define CONEXION_H_


#include"protocolo.h"


/* liberar_conexion:
 * Recibe un socket al que finalizaremos
 * */
void liberar_conexion(int socket);

/* levantar_servidor:
 * Recibe como parametros la ip y el puerto en donde se levantar el servidor
 * y el logger en donde registrar los sucesos
 *
 * Devuelve como entero el servidor levantado.
 * */

void levantar_servidor(char* ip, int puerto);

void esperar_cliente(int socket_servidor);

void servir_cliente(int socket);

void process_request(int id_modulo, int socket_cliente);

#endif /* CONEXION_H_ */
