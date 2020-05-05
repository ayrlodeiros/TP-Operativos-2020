#include "utils.h"

void inicializar_message_queues(){


	crear_get_mq();
	crear_localized_mq();
	crear_catch_mq();
	crear_caught_mq();
	crear_new_mq();
	crear_appeared_mq();

	printf("Message queues creadas correctamente \n");

}

void liberar_message_queues(){

	liberar_get_mq();
	liberar_localized_mq();
	liberar_catch_mq();
	liberar_caught_mq();
	liberar_new_mq();
	liberar_appeared_mq();

	printf("Se libero la memoria de los mq correctamente\n");
}

void crear_get_mq(){
	get_mq = malloc(sizeof(t_mq));
	get_mq->nombre = GET;
	get_mq->cola = queue_create();
	get_mq->suscriptores = list_create();
}

void crear_localized_mq(){
	localized_mq = malloc(sizeof(t_mq));
	localized_mq->nombre = LOCALIZED;
	localized_mq->cola = queue_create();
	localized_mq->suscriptores = list_create();

}

void crear_catch_mq(){
	catch_mq = malloc(sizeof(t_mq));
	catch_mq->nombre = CATCH;
	catch_mq->cola = queue_create();
	catch_mq->suscriptores = list_create();
}

void crear_caught_mq(){
	caught_mq = malloc(sizeof(t_mq));
	caught_mq->nombre = CAUGHT;
	caught_mq->cola = queue_create();
	caught_mq->suscriptores = list_create();
}
void crear_new_mq(){
	new_mq = malloc(sizeof(t_mq));
	new_mq->nombre = NEW;
	new_mq->cola = queue_create();
	new_mq->suscriptores = list_create();
}
void crear_appeared_mq(){
	appeared_mq = malloc(sizeof(t_mq));
	appeared_mq->nombre = APPEARED;
	appeared_mq->cola = queue_create();
	appeared_mq->suscriptores = list_create();
}

void liberar_get_mq(){
	list_destroy(get_mq->suscriptores);
	queue_destroy(get_mq->cola);
	free(get_mq);

}
void liberar_localized_mq(){
	list_destroy(localized_mq->suscriptores);
	queue_destroy(localized_mq->cola);
	free(localized_mq);
}
void liberar_catch_mq(){
	list_destroy(catch_mq->suscriptores);
	queue_destroy(catch_mq->cola);
	free(catch_mq);
}
void liberar_caught_mq(){
	list_destroy(caught_mq->suscriptores);
	queue_destroy(caught_mq->cola);
	free(caught_mq);
}
void liberar_new_mq(){
	list_destroy(new_mq->suscriptores);
	queue_destroy(new_mq->cola);
	free(new_mq);
}
void liberar_appeared_mq(){
	list_destroy(appeared_mq->suscriptores);
	queue_destroy(appeared_mq->cola);
	free(appeared_mq);
}
