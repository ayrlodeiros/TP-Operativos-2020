#ifndef UTILS_H_
#define UTILS_H_

#include"acciones.h"

int socket_broker;

pokemon* pokemon_para_planificar;

//CONEXIONES
t_paquete* crear_paquete(codigo_operacion cod_op, codigo_accion cod_acc, t_buffer* buffer);
t_paquete* crear_paquete_sin_buffer(codigo_operacion cod_op, codigo_accion cod_acc);
void* serializar_paquete(t_paquete* paquete, int tamanio_a_enviar);
void* serializar_paquete_sin_buffer(t_paquete* paquete, int tamanio_a_enviar);
int obtener_tamanio_de_paquete(t_paquete* paquete);
void destruir_paquete(t_paquete* paquete);
int crear_conexion_como_cliente(char *ip, char* puerto);
void liberar_conexion(int socket);
int levantar_servidor(char* ip, char* puerto);
int intentar_conectar_al_broker();
void levantar_conexiones_al_broker();
void esperar_mensaje_appeared();
void esperar_mensaje_localized();
void esperar_mensaje_caught();
void suscribirse_a_cola(int conexion_broker, codigo_accion cola_a_suscribir);
//FIN DE CONEXIONES

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado);

void manejar_aparicion_de_pokemon(char* nombre, int posicion_x, int posicion_y);
void buscar_entrenador_disponible();
void buscar_entrenador_a_planificar_para_moverse();
void agregar_entrenador_a_entrenadores_ready(entrenador* entrenador_listo, pokemon* pokemon_suelto);
void agregar_movimientos_en_x(entrenador* entrenador_listo, int diferencia_en_x);
void agregar_movimientos_en_y(entrenador* entrenador_listo, int diferencia_en_y);
void agregar_accion(entrenador* entrenador_listo, void* movimiento, int cpu_requerido);
int el_entrenador_se_puede_planificar(entrenador* un_entrenador);
int el_entrenador1_esta_mas_cerca(entrenador* entrenador1, entrenador* entrenador2);
int distancia_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);
void calcular_distancia_en_x_del_entrenador_a_la_posicion(entrenador* entrenador, posicion* posicion);
void calcular_distancia_en_y_del_entrenador_a_la_posicion(entrenador* entrenador, posicion* posicion);

int el_pokemon_es_requerido(char* nombre_pokemon);
int necesito_mas_de_ese_pokemon(char* nombre_pokemon);

//DEADLOCK
int necesito_mas_pokemons();
int el_entrenador_cumplio_su_objetivo(entrenador* entrenador);
int hay_deadlock();
int el_entrenador_esta_block_deadlock(entrenador* entrenador);
int cantidad_del_mismo_pokemon_por_entrenador(t_list* lista_de_pokemons,char* pokemon);
int tiene_mas_cantidad_de_ese_pokemon(t_list* pokemons_adquiridos, t_list* pokemons_objetivo, char* pokemon);
int es_el_pokemon_buscado(entrenador* entrenador1, char* pokemon_a_eliminar);
t_list* obtener_entrenadores_para_intercambiar(entrenador* entrenador1);
void planear_intercambio(entrenador* entrenador1);
t_list* entrenadores_con_block_deadlock();
int el_entrenador_esta_en_exit(entrenador* entrenador);
int terminaron_todos_los_entrenadores();
int el_entrenador_no_puede_capturar_mas_pokemons(entrenador* entrenador);
void intercambiar(entrenador* entrenador1);
void realizar_intercambio(entrenador* entrenador1, entrenador* entrenador_a_negociar);
t_list* obtener_pokemons_que_me_sobran(entrenador* entrenador);
t_list* obtener_pokemons_que_necesito(entrenador* entrenador);

//FIN DE DEADLOCK


//ACCIONES
void get_pokemon();
void realizar_get(char* key, void* value);
void esperar_id_localized(int socket_get);

void catch_pokemon(entrenador* entrenador);
void esperar_id_caught(int socket_catch);
void manejar_la_captura_del_pokemon(entrenador* entrenador);
void agregar_pokemon_a_adquirido(entrenador* entrenador, char* pokemon_adquirido);
void destruir_pokemon(pokemon* pokemon);


#endif /* UTILS_H_ */
