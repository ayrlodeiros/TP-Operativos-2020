#include "protocolo.h"


void agregar_suscriptor_cola(t_mq* cola,suscriptor_t* suscriptor){
	int tamanio_anterior = list_size(cola->suscriptores);
	list_add(cola->suscriptores,suscriptor);
	if(list_size(cola->suscriptores) > tamanio_anterior){

		log_info(mi_log,"SE AGREGO EL PROCESO %d COMO SUSCRIPTOR A LA COLA %d ",suscriptor->identificador,cola->nombre);

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

		int tamanio;
		int id_correlativo;
		void* buffer;
		recv(socket_cliente, &id_correlativo, sizeof(int), MSG_WAITALL);
		log_debug(mi_log,"El id del msj es :%d", id_correlativo);
		recv(socket_cliente, &tamanio, sizeof(int), MSG_WAITALL);
		buffer = malloc(tamanio);
		recv(socket_cliente, buffer, tamanio, MSG_WAITALL);

		//Crea el mensaje y ya lo guarda en memoria
		pthread_mutex_lock(&mutex_memoria_principal);
		t_mensaje* mensaje = crear_mensaje(buffer,tamanio,queue->nombre,id_correlativo);
		log_debug(mi_log, "SE ALMACENO EL MENSAJE DE ID: %d Y TAMANIO :%d EN LA POSICION INICIAL: %p", mensaje->id,mensaje->pos_en_memoria->tamanio, memoria_principal+(mensaje->pos_en_memoria->pos));
		agregar_a_lista_global(mensaje);
		if(!list_is_empty(queue->suscriptores)){
			agregar_msj_cola(queue,mensaje);
		}

		log_debug(mi_log,"Voy a mandar el id de msj: %d al socket %d",mensaje->id,socket_cliente);

		int id_mensaje = mensaje->id;
		pthread_mutex_unlock(&mutex_memoria_principal);

		enviar_id_msj_cliente(socket_cliente,id_mensaje);
}


void agregar_msj_cola(t_mq* queue,t_mensaje* mensaje){
	pthread_mutex_lock(&mutex_agregar_msj_a_cola);

	int tamanio_previo = list_size(queue->cola);   //Esta solo para confirmar que que se agrego correctamente el msj a la cola
	list_add(queue->cola,mensaje);
	if(list_size(queue->cola) > tamanio_previo)
		log_info(mi_log,"SE AGREGO UN NUEVO MENSAJE A LA COLA %d",queue->nombre);

	pthread_mutex_unlock(&mutex_agregar_msj_a_cola);

	pthread_mutex_unlock(&queue->lock);
}


void enviar_id_msj_cliente(int socket_cliente,int id_msj){
	bool se_mando = false;
	int contador = 3;
	while(!se_mando && contador != 0){
		if(send(socket_cliente,&id_msj, sizeof(int), 0) > 0){
			se_mando = true;
			log_info(mi_log,"Se envio el id del mensaje %d al suscriptor correspondiente correctamente", id_msj);
		}
		else{
			log_info(mi_log,"No se pudo mandar el id, se intentara nuevamente.......");
			contador--;
			sleep(10);
		}
	}

}

void switch_cola(int cod_op, int socket_cliente, int id_modulo){
	int cola;
	recv(socket_cliente,&cola,sizeof(int),MSG_WAITALL);
	log_debug(mi_log,"El proceso de socket %d se quiere conectar a la cola %d.",socket_cliente,cola);
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
				log_debug(mi_log,"Hubo un error al tratar de recibir el mensaje, no se encontro la cola");
				pthread_exit(NULL);
	}
}

void switch_operacion (op_code operacion, t_mq* cola,int conexion, int id_modulo){
	suscriptor_t* suscriptor;
	switch(operacion){
	case MENSAJE:
		log_debug(mi_log, "Se recibio una conexion con el modulo de id: %d para enviar un MENSAJE a la cola %d", id_modulo, cola->nombre);
		recibir_y_guardar_mensaje(conexion, cola);
		break;
	case SUSCRIPCION:
		log_debug(mi_log, "Se recibio una conexion con el modulo de id: %d para SUSCRIBIRSE a la cola %d", id_modulo, cola->nombre);
		recibir_suscriptor(conexion, id_modulo, cola);
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
