#ifndef UTILS_H_
#define UTILS_H_

#include"constructor.h"
#include"config-reader.h"
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
int socket_broker;

void conectar_broker(void);

void armar_entrenadores(void);
entrenador* armar_entrenador(char* posicion, char* pokemons, char * objetivos);
posicion* armar_posicion(char* posicion_a_armar);

void armar_objetivo_global(void);
void agregar_objetivo_a_objetivo_global(char* pokemon_objetivo);
void restar_adquirido_a_objetivo_global(char* pokemon_adquirido);

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado);
int el_entrenador_se_puede_planificar(entrenador* un_entrenador);
entrenador* entrenador_a_ejecutar(void);
int distancia_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);
int el_entrenador1_esta_mas_cerca(entrenador* entrenador1, entrenador* entrenador2);
int hay_deadlock(entrenador* entrenador1, entrenador* entrenador2);
void restar_cpu_disponible(entrenador* entrenador, int cantidad);
void sumar_cpu_usado(entrenador* entrenador, int cantidad);
int el_pokemon_es_requerido(char* nombre_pokemon);
int esta_en_el_objetivo_global(char* nombre_pokemon);
int necesito_mas_de_ese_pokemon(char* nombre_pokemon);


#endif /* UTILS_H_ */
