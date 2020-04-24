#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

#include<commons/config.h>

t_config* config;

void iniciar_config(void);
t_config* get_config(void);
void destruir_config(void);


//HACER ESTOS TRES MAS ADELANTE
void leer_posiciones_entrenadores(void);
void leer_pokemon_entrenadores(void);
void leer_objetivos_entrenadores(void);

int leer_tiempo_reconexion(void);

int leer_retardo_ciclo_cpu(void);

char* leer_algoritmo_planificacion(void);

int leer_quantum(void);

int leer_estimacion_inicial(void);

char* leer_ip_broker(void);

char* leer_puerto_broker(void);

char* leer_log_file(void);

#endif /* CONFIGREADER_H_ */
