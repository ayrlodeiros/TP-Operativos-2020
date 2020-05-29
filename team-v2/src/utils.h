#ifndef UTILS_H_
#define UTILS_H_

#include"acciones.h"

int socket_broker;

pokemon* pokemon_para_planificar;

//CONEXIONES
int crear_conexion_como_cliente(char *ip, char* puerto);
void liberar_conexion(int socket);
int levantar_servidor(char* ip, char* puerto);
void conectar_broker(int conexion);
void intentar_conectar_al_broker(int conexion);
//FIN DE CONEXIONES

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado);

void manejar_aparicion_de_pokemon(char* nombre, int posicion_x, int posicion_y);
void buscar_entrenador_a_planificar_para_moverse();
void agregar_entrenador_a_entrenadores_ready(entrenador* entrenador_listo, pokemon* pokemon_suelto);
void agregar_movimientos_en_x(entrenador* entrenador_listo);
void agregar_movimientos_en_y(entrenador* entrenador_listo);
void agregar_accion(entrenador* entrenador_listo, void* movimiento, int cpu_requerido);
int el_entrenador_se_puede_planificar(entrenador* un_entrenador);
int el_entrenador1_esta_mas_cerca(entrenador* entrenador1, entrenador* entrenador2);
int distancia_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);
int diferencia_en_x_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);
int diferencia_en_y_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);

int el_pokemon_es_requerido(char* nombre_pokemon);
int necesito_mas_de_ese_pokemon(char* nombre_pokemon);

void atrapar_pokemon(entrenador* entrenador);
void manejar_la_captura_del_pokemon(entrenador* entrenador);
void destruir_pokemon(pokemon* pokemon);


#endif /* UTILS_H_ */
