#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include "constructor.h"


/* Funciones sobre suscriptores */
void agregar_suscriptor_cola(t_mq* cola,suscriptor_t* suscriptor);

void recibir_suscriptor(int conexion, int id_modulo, t_mq* cola);

int chequear_si_ya_existe_suscriptor(t_list* suscriptores,int id_nuevo);

/**Tambien se podria separar en dos funciones */
void recibir_y_guardar_mensaje(int socket_cliente,t_mq* queue);


void agregar_msj_cola(t_mq* cola,t_mensaje* mensaje);

void enviar_mensaje_suscriptores(t_mq* cola);

void enviar_id_msj_cliente(int socket_cliente,int id_msj);

/* Filtra los mensajes de la lista global y los agrega a la cola correspondiente*/
void mandar_mensajes_cache(t_mq* cola);


/*Switchs*/
void switch_cola(int cod_op, int socket_cliente, int id_modulo);
void switch_operacion (op_code operacion, t_mq* cola,int conexion, int id_modulo);


#endif // PROTOCOLO_H_
