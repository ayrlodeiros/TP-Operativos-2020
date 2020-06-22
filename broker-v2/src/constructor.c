#include "constructor.h"

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

	liberar_mq(get_mq);
	liberar_mq(localized_mq);
	liberar_mq(catch_mq);
	liberar_mq(caught_mq);
	liberar_mq(new_mq);
	liberar_mq(appeared_mq);

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

void liberar_mq(t_mq* mq){
	list_destroy(mq->suscriptores);
	queue_destroy(mq->cola);
	free(mq);
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

/*Por ahora solo crea una estrucutura t_mensaje con algunos valores, no todos*/
t_mensaje* crear_mensaje(void* buffer,int tamanio,mq_nombre cola){

	int posicion;
	t_mensaje* mensaje  = malloc(sizeof(t_mensaje));
	/*todo Modificar la funcion para que se adapte a la memoria*/
	guardar_mensaje_en_memoria(void* msj,&posicion,tamanio);
	mensaje->id = asignar_id_univoco();
	mensaje->cola = cola;
	mensaje->pos_en_memoria->tamanio = tamanio;
	mensaje->pos_en_memoria->pos = posicion;

	return mensaje;
}

/*Inicializa el contador de ids */
/*todo IDS MENSAJES */
void iniciar_contador_ids_mensaje(){
	contador_ids_mensaje = 1;
}

int asignar_id_univoco(){
	int valor = contador_ids_mensaje;
	contador_ids_mensaje++;
	return valor;
}

void iniciar_memoria_principal(){
	memoria_principal = malloc(leer_memoria_principal());
	ultima_pos = 0;
}

void guardar_mensaje_en_memoria(int tamanio,void* buffer,int* posicion){

	switch(leer_algoritmo_principal()){
		case PARTICIONES:
			break;
		case BS:
			break;
		case NORMAL:
			/*todo si hay error puede que sea q falta el * en posicion */
				guardar_mensaje_normal(tamanio,buffer,posicion);
			break;
	}
}

void guardar_mensaje_normal(int tamanio,void* buffer,int*posicion){

	*posicion = ultima_pos;
	memcpy();
}
