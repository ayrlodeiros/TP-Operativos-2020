#ifndef CONEXIONES_H_
#define CONEXIONES_H_


/* ACLARACIONES DE FALTANTE
 *
 * Agregar id_modulo al config y al config-reader
 * y agrega ip_gamecard y puerto_gamecard
 * Ver el Para GAMEBOY
 *
 * */

#include "bitmap.h"

typedef enum
{
	MENSAJE = 1,
	SUSCRIPCION = 2
}codigo_operacion;

typedef enum
{
	GET = 1,
	LOCALIZED = 2,
	CATCH = 3,
	CAUGHT = 4,
	NEW = 5,
	APPEARED = 6
}codigo_accion;

typedef struct
{
	int tamanio;
	void* stream;
} t_buffer;

typedef struct
{
	int numero_de_modulo;
	codigo_operacion codigo_de_operacion;
	codigo_accion codigo_de_accion;
	int id_mensaje;
	t_buffer* buffer;
} t_paquete;

typedef struct
{
	int id;
	int id_correlativo;
	int tamanio;
	void* payload;
} mensaje_broker;


int conexion_new;
int conexion_catch;
int conexion_get;
int funciona_broker;
pthread_mutex_t lock_reintento_broker;
pthread_mutex_t mutex_funciona_broker;


void levantar_conexiones();
t_paquete* crear_paquete(codigo_operacion cod_op, codigo_accion cod_acc, int id_mensaje, t_buffer* buffer);
t_paquete* crear_paquete_sin_buffer(codigo_operacion cod_op, codigo_accion cod_acc);
void* serializar_paquete(t_paquete* paquete, int tamanio_a_enviar);
void* serializar_paquete_sin_buffer(t_paquete* paquete, int tamanio_a_enviar);
int obtener_tamanio_de_paquete(t_paquete* paquete);
void destruir_paquete(t_paquete* paquete);
int crear_conexion_como_cliente(char *ip, int puerto_i);
void liberar_conexion(int socket);
int levantar_servidor(char* ip, char* puerto);

void atender_conexion_gameboy();
void recibir_mensaje_de_gameboy(int socket_gameboy);
void procesar_mensaje_new(int hubo_error, int socket_gameboy, int id_mensaje);
void procesar_mensaje_catch(int hubo_error, int socket_gameboy, int id_mensaje);
void procesar_mensaje_get(int hubo_error, int socket_gameboy, int id_mensaje);
t_list* armar_lista_de_posiciones_del_pokemon(t_list* datos);

int intentar_conectar_al_broker();
void levantar_conexiones_al_broker();

//NEW Y APPEARED
void esperar_mensaje_new();
void trabajar_mensaje_new(mensaje_broker* msj_broker);
void enviar_mensaje_appeared(int id_mensaje, int largo_nombre_pokemon, char* nombre_pokemon, int posicion_x, int posicion_y);

//CATCH Y CAUGHT
void esperar_mensaje_catch();
void trabajar_mensaje_catch(mensaje_broker* msj_broker);
void enviar_mensaje_caught(int id_mensaje, int resultado);

//GET Y LOCALIZED
void esperar_mensaje_get();
void trabajar_mensaje_get(mensaje_broker* msj_broker);
void enviar_mensaje_localized(int id_mensaje, int largo_nombre_pokemon, char* nombre_pokemon, int cantidad_de_posiciones, t_list* posiciones);

mensaje_broker* recibir_msj_broker(int conexion_broker);
void mandar_ack(int conexion, int resultado);
void suscribirse_a_cola(int conexion_broker, codigo_accion cola_a_suscribir);
void cambiar_valor_de_funciona_broker(int new_value);
void desbloquear_lock_reintento();


#endif /* CONEXIONES_H_ */
