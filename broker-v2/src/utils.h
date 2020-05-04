#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


t_log* logger;
t_log* mi_log;

typedef struct
{
	int GET;
	int LOCALIZED;
	int CATCH;
	int CAUGHT;
	int NEW;
	int APPEARED;
}t_cola;

t_cola COLA;

//----Metodos para crear las colas de mensajes ------
void inicializar_message_queues(void);
void terminar_message_queues(void);
void crear_get_mq(key_t key);
void crear_localized_mq(key_t key);
void crear_catch_mq(key_t key);
void crear_caught_mq(key_t key);
void crear_new_mq(key_t key);
void crear_appeared_mq(key_t key);

#endif /* UTILS_H_ */
