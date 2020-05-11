#ifndef UTILS_H_
#define UTILS_H_

#include"acciones.h"

int socket_broker;

static pthread_mutex_t lock_de_conexion_broker = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock_de_planificacion = PTHREAD_MUTEX_INITIALIZER;

pokemon* pokemon_para_planificar;

//CONEXIONES
int crear_conexion_como_cliente(char *ip, char* puerto);
void liberar_conexion(int socket);
int levantar_servidor(char* ip, char* puerto);
void conectar_broker(void);
int funciona_la_conexion_con_broker();
void intentar_conectar_al_broker();
//FIN DE CONEXIONES

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado);

void manejar_aparicion_de_pokemon(char* nombre, int posicion_x, int posicion_y);

void buscar_entrenador_a_planificar();
void agregar_entrenador_a_entrenadores_ready(entrenador* entrenador_listo);
int el_entrenador_se_puede_planificar(entrenador* un_entrenador);
int el_entrenador1_esta_mas_cerca(entrenador* entrenador1, entrenador* entrenador2);
int distancia_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);
void sumar_cpu_usado(entrenador* entrenador, int cantidad);

int el_pokemon_es_requerido(char* nombre_pokemon);
int necesito_mas_de_ese_pokemon(char* nombre_pokemon);


#endif /* UTILS_H_ */
