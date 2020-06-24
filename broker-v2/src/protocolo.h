#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include "constructor.h"

void agregar_suscriptor_cola(t_mq* cola,suscriptor_t* suscriptor);

/**Tambien se podria separar en dos funciones */
void recibir_y_guardar_mensaje(int socket_cliente,t_mq* queue);


void agregar_msj_cola(t_mq* cola,t_mensaje* mensaje);

void enviar_mensaje_suscriptores(t_mq* cola);

/** Crea el paquete para manda en funcion del mensaje */
void enviar_mensaje(t_mensaje* mensaje,suscriptor_t* suscriptor);

void* serializar_paquete(t_paquete* paquete, int bytes);

void enviar_id_msj_cliente(int socket_cliente,t_mq* mq,int id_msj);

/* Falta definir los siguientes metodos */

void recibir_ACK(suscriptor_t* socket_cliente,t_mensaje* mensaje);

/*Al suscribirse un modulo, se le tienen que mandar todos los mensajes correspondientes */
void mandar_mensajes_cache();
void add_sub_lista_env_msj(t_mensaje* mensaje,suscriptor_t* suscriptor);
void add_sub_lista_conf_msj(t_mensaje* mensaje,suscriptor_t* suscriptor);


/*Switchs*/
void switch_cola(int cod_op, int socket_cliente, int id_modulo);
void switch_operacion (op_code operacion, t_mq* cola,int conexion, modulo_code modulo);


#endif // PROTOCOLO_H_
