#ifndef MESSAGE_QUEUE_
#define MESSAGE_QUEUE_


#include<commons/collections/queue.h>
#include<commons/collections/list.h>

typedef enum{
		GET = 1,
		LOCALIZED = 2,
		CATCH = 3,
		CAUGHT = 4,
		NEW = 5 ,
		APPEARED = 6
}mq_nombre;

typedef struct{
	mq_nombre nombre;
	t_queue* cola;
	t_list* suscriptores; /** Lo cambie a una estructura suscriptor definida en protocolo.h*/

}t_mq;

/**
 * Despues veo si armo este struct

typedef struct{
	t_mq* GET;
	//t_mq* LOCALIZED;
	//t_mq* CATCH;
	//t_mq* NEW;
	//t_mq* APPEARED;
}t_colas;
*/

t_mq* get_mq;
t_mq* localized_mq;
t_mq* catch_mq;
t_mq* caught_mq;
t_mq* new_mq;
t_mq* appeared_mq;


/** Metodos para crear las colas de mensajes */
void inicializar_message_queues(void);
void liberar_message_queues(void);
void crear_get_mq(void);
void crear_localized_mq(void);
void crear_catch_mq(void);
void crear_caught_mq(void);
void crear_new_mq(void);
void crear_appeared_mq(void);

void liberar_get_mq(void);
void liberar_localized_mq(void);
void liberar_catch_mq(void);
void liberar_caught_mq(void);
void liberar_new_mq(void);
void liberar_appeared_mq(void);

#endif /* UTILS_H_ */
