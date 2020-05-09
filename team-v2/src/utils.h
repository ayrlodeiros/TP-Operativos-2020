#ifndef UTILS_H_
#define UTILS_H_

#include"constructor.h"
#include"config-reader.h"
#include<sys/socket.h>
#include<netdb.h>
#include<signal.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

t_log* logger;
t_log* nuestro_log;

pthread_mutex_t lock_de_conexion_broker;
int socket_broker;

//CONEXIONES
int crear_conexion_como_cliente(char *ip, char* puerto);
void liberar_conexion(int socket);
int levantar_servidor(char* ip, char* puerto);
void conectar_broker(void);
int funciona_la_conexion_con_broker();
//FIN DE CONEXIONES

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado);
int el_entrenador_se_puede_planificar(entrenador* un_entrenador);
entrenador* entrenador_a_ejecutar(void);
int distancia_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);
int el_entrenador1_esta_mas_cerca(entrenador* entrenador1, entrenador* entrenador2);
void restar_cpu_disponible(entrenador* entrenador, int cantidad);
void sumar_cpu_usado(entrenador* entrenador, int cantidad);
int el_pokemon_es_requerido(char* nombre_pokemon);
int necesito_mas_de_ese_pokemon(char* nombre_pokemon);


#endif /* UTILS_H_ */
