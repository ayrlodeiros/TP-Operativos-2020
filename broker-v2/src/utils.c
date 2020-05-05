#include "utils.h"

void conectar_team(void){

}

void inicializar_message_queues(){

	key_t key;
	crear_get_mq(key);
	crear_localized_mq(key);
	crear_catch_mq(key);
	crear_caught_mq(key);
	crear_new_mq(key);
	crear_appeared_mq(key);
	printf("Message queues creadas correctamente \n");

}

void terminar_message_queues(){
    msgctl(COLA.GET, IPC_RMID, NULL);
    msgctl(COLA.LOCALIZED, IPC_RMID, NULL);
    msgctl(COLA.CATCH, IPC_RMID, NULL);
    msgctl(COLA.CAUGHT, IPC_RMID, NULL);
    msgctl(COLA.NEW, IPC_RMID, NULL);
    msgctl(COLA.APPEARED, IPC_RMID, NULL);
    printf("Destruyendo todas las colas de mensajes");
}

void crear_get_mq(key_t key){
	key = ftok("GET",1);
	COLA.GET = msgget(key, 0666 | IPC_CREAT);
}

void crear_localized_mq(key_t key){
	key = ftok("LOCALIZED",1);
	COLA.LOCALIZED = msgget(key,0666 | IPC_CREAT);
}
void crear_catch_mq(key_t key){
	key = ftok("CATCH",1);
	COLA.CATCH = msgget(key,0666 | IPC_CREAT);
}
void crear_caught_mq(key_t key){
	key = ftok("CAUGHT",1);
	COLA.CAUGHT = msgget(key, 0666 | IPC_CREAT);
}
void crear_new_mq(key_t key){
	key = ftok("NEW",1);
	COLA.NEW = msgget(key, 0666 | IPC_CREAT);
}
void crear_appeared_mq(key_t key){
	key = ftok("APPEARED",1);
	COLA.APPEARED = msgget(key, 0666 | IPC_CREAT);
}
