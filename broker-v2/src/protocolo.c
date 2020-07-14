#include "protocolo.h"


void agregar_suscriptor_cola(t_mq* cola,suscriptor_t* suscriptor){
	int tamanio_anterior = list_size(cola->suscriptores);
	list_add(cola->suscriptores,suscriptor);
	if(list_size(cola->suscriptores) > tamanio_anterior){
			log_info(mi_log,"Se agrego el suscriptor %d a la cola %d correctamente\n",suscriptor->identificador,cola->nombre); /** Por ahora le paso la conexion
			 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	como nombre puede que cambie a futuro*/
	}

	mandar_mensajes_cache(cola);
}


void mandar_mensajes_cache(t_mq* cola){

	pthread_mutex_lock(&mutex_lista_msjs);
	for(int i=0; i<list_size(lista_global_msjs); i++) {
		t_mensaje* mensaje = list_get(lista_global_msjs, i);
		if(mensaje->cola == cola->nombre) {
			agregar_msj_cola(cola, mensaje);
		}
	}
	pthread_mutex_unlock(&mutex_lista_msjs);
}


void recibir_y_guardar_mensaje(int socket_cliente,t_mq* queue){

		/*todo adaptar a distintos tipos de mensajes */
		int tamanio;
		int id_correlativo;
		void* buffer;
		recv(socket_cliente, &id_correlativo, sizeof(int), MSG_WAITALL);
		log_info(mi_log,"El id del msj es :%d", id_correlativo);
		recv(socket_cliente, &tamanio, sizeof(int), MSG_WAITALL);
		log_info(mi_log,"El tamanio del msj es :%d", tamanio);
		buffer = malloc(tamanio);
		recv(socket_cliente, buffer, tamanio, MSG_WAITALL);
		log_info(mi_log, "Se recibio el mensaje correctamente\n");

		//Crea el mensaje y ya lo guarda en memoria
		t_mensaje* mensaje = crear_mensaje(buffer,tamanio,queue->nombre,id_correlativo);
		agregar_a_lista_global(mensaje);
		if(!list_is_empty(queue->suscriptores)){
			agregar_msj_cola(queue,mensaje);
		}
		log_info(mi_log,"Voy a mandar el id de msj: %d al socket %d",mensaje->id,socket_cliente);
		enviar_id_msj_cliente(socket_cliente,mensaje->id);
}


void agregar_msj_cola(t_mq* queue,t_mensaje* mensaje){
	pthread_mutex_lock(&mutex_agregar_msj_a_cola);

	int tamanio_previo = queue_size(queue->cola);   //Esta solo para confirmar que que se agrego correctamente el msj a la cola
	queue_push(queue->cola,mensaje);
	if(queue_size(queue->cola) > tamanio_previo)
		log_info(mi_log,"Se agrego correctamente el mensaje a la cola %d",queue->nombre);

	pthread_mutex_unlock(&mutex_agregar_msj_a_cola);

	pthread_mutex_unlock(&queue->lock);
}

/*
void enviar_mensaje_suscriptores(t_mq* cola){
	pthread_t* ack;
	t_mensaje* mensaje = queue_pop(cola->cola);
	suscriptor_t* suscriptor;
	int i;

	for(i=0;i<list_size(cola->suscriptores);i++){
		suscriptor = list_get(cola->suscriptores,i);

		if(pthread_create(&ack,NULL,(void*)enviar_mensaje,&mensaje,&suscriptor) != 0)
		{
			log_info(mi_log,"Hubo un error al intentar crear el thread");
		}
		pthread_detach(ack);
	}
	log_info(mi_log,"Se envio el mensaje a todos los suscriptores \n");
}*/


void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->id), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, &(paquete->id_cor), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}


void enviar_id_msj_cliente(int socket_cliente,int id_msj){
	//todo Implementar caso que el send falle
	if(send(socket_cliente,&id_msj, sizeof(int), 0) > 0){
			log_info(mi_log,"Se envio el id del mensaje correctamente\n");
		}
}

void switch_cola(int cod_op, int socket_cliente, int id_modulo){
	int cola;
	recv(socket_cliente,&cola,sizeof(int),MSG_WAITALL);
	log_info(mi_log,"El socket %d se quiere conectar a la cola %d.",socket_cliente,cola);
	switch (cola){
			case GET:
				switch_operacion(cod_op,get_mq,socket_cliente,id_modulo);
				break;
			case LOCALIZED:
				switch_operacion(cod_op,localized_mq,socket_cliente,id_modulo);
				break;
			case CATCH:
				switch_operacion(cod_op,catch_mq,socket_cliente,id_modulo);
				break;
			case CAUGHT:
				switch_operacion(cod_op,caught_mq,socket_cliente,id_modulo);
				break;
			case NEW:
				switch_operacion(cod_op,new_mq,socket_cliente,id_modulo);
				break;
			case APPEARED:
				switch_operacion(cod_op,appeared_mq,socket_cliente,id_modulo);
				break;
			default:
				log_info(mi_log,"HUbo un error al tratar de recibir el mensaje\n");
				pthread_exit(NULL);
	}
}

void switch_operacion (op_code operacion, t_mq* cola,int conexion, int id_modulo){
	suscriptor_t* suscriptor;
	switch(operacion){
	case MENSAJE:
		recibir_y_guardar_mensaje(conexion, cola);
		break;
	case SUSCRIPCION:
		recibir_suscriptor(conexion, id_modulo, cola);
		//TODO ver como manejar la llegada de un nuevo suscriptor para enviarle los mensajes de cache
		break;
	}
}

void recibir_suscriptor(int conexion, int id_modulo, t_mq* cola){
	int posicion = chequear_si_ya_existe_suscriptor(cola->suscriptores,id_modulo);

	suscriptor_t* suscriptor;

	if(posicion > -1){
		suscriptor = list_get(cola->suscriptores, posicion);
		suscriptor->conexion = conexion;
	} else {
		suscriptor = crear_suscriptor(conexion,id_modulo);
		agregar_suscriptor_cola(cola, suscriptor);
	}

}

int chequear_si_ya_existe_suscriptor(t_list* suscriptores,int id_nuevo){

	for(int i = 0; i<list_size(suscriptores);i++){
		suscriptor_t* suscriptor = list_get(suscriptores,i);
		if(id_nuevo == suscriptor->identificador){
			return i;
		}
	}

	return -1;

}
