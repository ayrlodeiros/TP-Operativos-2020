#ifndef UTILS_H_
#define UTILS_H_

#include"acciones.h"

int socket_broker;

pokemon* pokemon_para_planificar;

//CONEXIONES
<<<<<<< HEAD
int crear_conexion_como_cliente(char *ip, char* puerto);
void liberar_conexion(int socket);
int levantar_servidor(char* ip, char* puerto);
void conectar_broker(int conexion);
void intentar_conectar_al_broker(int conexion);
=======
t_paquete* crear_paquete(codigo_operacion cod_op, codigo_accion cod_acc, t_buffer* buffer);
t_paquete* crear_paquete_sin_buffer(codigo_operacion cod_op, codigo_accion cod_acc);
void* serializar_paquete(t_paquete* paquete, int tamanio_a_enviar);
void* serializar_paquete_sin_buffer(t_paquete* paquete, int tamanio_a_enviar);
int obtener_tamanio_de_paquete(t_paquete* paquete);
void destruir_paquete(t_paquete* paquete);
int crear_conexion_como_cliente(char *ip, char* puerto);
void liberar_conexion(int conexion);
int levantar_servidor(char* ip, char* puerto);
	//PARA GAMEBOY
void atender_conexion_gameboy();
void recibir_mensaje_de_gameboy(int socket_gameboy);
	//FIN DE PARA GAMEBOY
	//PARA BROKER
int intentar_conectar_al_broker();
void levantar_conexiones_al_broker();
mensaje_broker* recibir_msj_broker(int conexion_broker);
void mandar_ack(int conexion, int resultado);
void esperar_mensaje_appeared();
void esperar_mensaje_localized();
int pokemon_ya_fue_recibido(char* pokemon);
void actualizar_pokemon_como_recibido(char* pokemon);
int id_esta_en_lista_ids_localized(int id);
void esperar_mensaje_caught();
int obtener_posicion_en_lista_de_id_caught(int id_caught);
void suscribirse_a_cola(int conexion_broker, codigo_accion cola_a_suscribir);
void cambiar_valor_de_funciona_broker(int new_value);
void desbloquear_lock_reintento();
	//FIN DE PARA BROKER
>>>>>>> develop
//FIN DE CONEXIONES

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado);

void manejar_aparicion_de_pokemon(char* nombre, int posicion_x, int posicion_y);
<<<<<<< HEAD
void buscar_entrenador_a_planificar_para_moverse();
void agregar_entrenador_a_entrenadores_ready(entrenador* entrenador_listo, pokemon* pokemon_suelto);
void agregar_movimientos_en_x(entrenador* entrenador_listo);
void agregar_movimientos_en_y(entrenador* entrenador_listo);
=======
void agregar_pokemon_a_pokemons_sin_entrenador(pokemon* nuevo_pokemon);
pokemon* quitar_pokemon_de_pokemons_sin_entrenador();
void agregar_pokemon_a_pokemons_en_espera(pokemon* nuevo_pokemon);
pokemon* quitar_pokemon_de_pokemons_en_espera(int posicion);
void buscar_entrenador_disponible();
void buscar_entrenador_a_planificar_para_moverse();
void agregar_entrenador_a_entrenadores_ready(entrenador* entrenador_listo, pokemon* pokemon_suelto);
void agregar_entrenador_a_lista_entrenadores_ready(entrenador* entrenador_listo);
void agregar_movimientos_en_x(entrenador* entrenador_listo, int diferencia_en_x);
void agregar_movimientos_en_y(entrenador* entrenador_listo, int diferencia_en_y);
>>>>>>> develop
void agregar_accion(entrenador* entrenador_listo, void* movimiento, int cpu_requerido);
int el_entrenador_se_puede_planificar(entrenador* un_entrenador);
int el_entrenador1_esta_mas_cerca(entrenador* entrenador1, entrenador* entrenador2);
int distancia_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);
<<<<<<< HEAD
int diferencia_en_x_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);
int diferencia_en_y_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon);
=======
void calcular_distancia_en_x_del_entrenador_a_la_posicion(entrenador* entrenador, posicion* posicion);
void calcular_distancia_en_y_del_entrenador_a_la_posicion(entrenador* entrenador, posicion* posicion);
>>>>>>> develop

int el_pokemon_es_requerido(char* nombre_pokemon);
int necesito_mas_de_ese_pokemon(char* nombre_pokemon);

//DEADLOCK
int el_entrenador_cumplio_su_objetivo(entrenador* entrenador);
int el_entrenador_esta_block_deadlock(entrenador* entrenador);
int cantidad_del_mismo_pokemon_por_entrenador(t_list* lista_de_pokemons,char* pokemon);
int tiene_mas_cantidad_de_ese_pokemon(t_list* pokemons_adquiridos, t_list* pokemons_objetivo, char* pokemon);
int se_encontraron_entrenadores_para_intercambio(entrenador* entrenador1, intercambio* un_intercambio);
void planear_intercambio(entrenador* entrenador1);
void sumar_uno_a_cantidad_de_deadlocks();
int el_otro_entrenador_tiene_el_pokemon_que_necesito(entrenador* entrenador1,entrenador* entrenador_a_evaluar);
char* pokemon_que_sirven_para_intercambiar(entrenador* entrenador1, entrenador* entrenador2);
t_list* pokemons_a_intercambiar_ideal(entrenador* entrenador1,entrenador* entrenador2);
t_list* entrenadores_con_block_deadlock();
int el_entrenador_esta_en_exit(entrenador* entrenador);
int terminaron_todos_los_entrenadores();
int el_entrenador_no_puede_capturar_mas_pokemons(entrenador* entrenador);
void intercambiar(entrenador* entrenador1);
void destruir_intercambio(intercambio* intercambio_realizado);
void realizar_intercambio(entrenador* entrenador1, entrenador* entrenador_a_negociar);
intercambio* buscar_intercambio_correspondiente_al_entrenador(entrenador* entrenador);

//FIN DE DEADLOCK


//ACCIONES
void get_pokemon();
void realizar_get(char* key, void* value);
void esperar_id_localized(int socket_get);

void catch_pokemon(entrenador* entrenador);
void esperar_id_caught(socket_y_entrenador* sye);
void manejar_la_no_captura_del_pokemon(entrenador* entrenador);
void manejar_la_captura_del_pokemon(entrenador* entrenador);
void accionar_en_funcion_del_estado_del_entrenador(entrenador* entrenador);
void agregar_pokemon_a_adquirido(entrenador* entrenador, char* pokemon_adquirido);
void destruir_pokemon(pokemon* pokemon);
void evaluar_pokemons_en_espera(char* nombre);


void mostrar_metricas(t_log* log);
int calcular_ciclos_de_CPU_totales();
void mostrar_ciclos_de_CPU_por_entrenador(t_log* log);

#endif /* UTILS_H_ */
