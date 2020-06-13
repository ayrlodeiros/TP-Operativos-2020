#include "protocolo.h"


void agregar_suscriptor_cola(t_mq* cola,suscriptor_t* suscriptor){
	int tamanio_anterior = list_size(cola->suscriptores);
	list_add(cola->suscriptores,suscriptor);
	if(list_size(cola->suscriptores) > tamanio_anterior){
			log_info(mi_log,string_from_format("Se agrego el suscriptor %d a la cola %d correctamente\n",suscriptor->conexion,cola->nombre)); /** Por ahora le paso la conexion
			 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	como nombre puede que cambie a futuro*/
	}
	/* Falta que al agregar un suscriptor se le envien todos los mensajes de la cola que esten en la cache
	enviar_msjs_cola_suscriptor();
	*/
}

void recibir_y_guardar_mensaje(int socket_cliente,t_mq* queue){

		int tamanio;
		void* buffer;
		recv(socket_cliente, tamanio, sizeof(int), MSG_WAITALL);
		buffer = malloc(tamanio);
		recv(socket_cliente, buffer, tamanio, MSG_WAITALL);
		log_info(mi_log, "Se recibio el mensaje\n");

		t_mensaje* mensaje = crear_mensaje(buffer,tamanio,queue->nombre);
		agregar_msj_cola(queue,mensaje);
		enviar_id_msj_cliente(socket_cliente,queue,mensaje->id);
}

/*Por ahora solo crea una estrucutura t_mensaje con algunos valores, no todos*/
t_mensaje* crear_mensaje(void* buffer,int tamanio,mq_nombre cola){

	t_mensaje* mensaje  = malloc(sizeof(t_mensaje));
	mensaje->buffer = malloc(sizeof(t_buffer));
	mensaje->id = asignar_id_univoco();
	mensaje->cola = cola;
	mensaje->buffer->size = tamanio;
	mensaje->buffer->stream = malloc(mensaje->buffer->size);
	memcpy(mensaje->buffer->stream,buffer,mensaje->buffer->size);

	return mensaje;
}

void agregar_msj_cola(t_mq* queue,t_mensaje* mensaje){
	int tamanio_previo = queue_size(queue->cola);   //Esta solo para confirmar que que se agrego correctamente el msj a la cola

	queue_push(queue->cola,mensaje);
	if(queue_size(queue->cola) > tamanio_previo)
		log_info(mi_log,string_from_format("Se agrego correctamente el mensaje a la cola %d",queue->nombre));
}

void enviar_mensaje_suscriptores(t_mq* cola){
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
}

void enviar_mensaje(t_mensaje* mensaje, suscriptor_t* cliente)
{
	t_paquete* paquete= malloc(sizeof(paquete));

	paquete->id = mensaje->id;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = mensaje->buffer->size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje->buffer->stream, paquete->buffer->size);

	int bytes = paquete->buffer->size + 3*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	if(send(cliente->conexion, a_enviar, bytes, 0) > 0){
		add_sub_lista_env_msj(mensaje,cliente);
		log_info(mi_log,"Se envio correctamente el mensaje al suscriptor\n");
	}
	else
		log_info(mi_log,"NO se envio correctamente el mensaje al suscriptor\n");

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	/* Chequear si conviene llamar esta funcion por separado */
	recibir_ACK(cliente,mensaje);
}

void recibir_ACK(suscriptor_t* suscriptor,t_mensaje* mensaje){
	int valor;
	if (recv(suscriptor->conexion,valor, sizeof(int), MSG_WAITALL) < 0){
		log_info(mi_log,"No se recibio la confirmacion de envio del mensaje");
		/* Deberia actualizar el mensaje e indicar que no se recibio la confirmacion de recepcion*/
	}
	else
		add_sub_lista_conf_msj(mensaje,suscriptor);
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->id), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->id_cor), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int asignar_id_univoco(){
	int valor = contador_ids_mensaje;
	contador_ids_mensaje++;
	return valor;
}

void enviar_id_msj_cliente(int socket_cliente,t_mq* cola,int id_msj){
	if(send(socket_cliente,id_msj, sizeof(int), 0) > 0){
			log_info(mi_log,"Se envio el id del mensaje correctamente\n");
		}
}

void mandar_mensajes_cache();


void add_sub_lista_conf_msj(t_mensaje* mensaje, suscriptor_t* suscriptor){
	list_add(mensaje->suscriptores_conf,suscriptor);
}

void add_sub_lista_env_msj(t_mensaje* mensaje,suscriptor_t* suscriptor){
	list_add(mensaje->suscriptores_env,suscriptor);
}
