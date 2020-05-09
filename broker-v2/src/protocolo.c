#include "protocolo.h"

void agregar_suscriptor_cola(t_mq* cola,int suscriptor){
	int tamanio_anterior = list_size(cola->suscriptores);
	list_add(cola->suscriptores,suscriptor);
	if(list_size(cola->suscriptores) > tamanio_anterior){
			log_info(mi_log,"Se agrego el suscriptor a la cola correctamente");
	}
}

void recibir_y_guardar_mensaje(int socket_cliente,t_mq* queue){

		int tamanio;
		void* buffer;
		recv(socket_cliente, tamanio, sizeof(int), MSG_WAITALL);
		buffer = malloc(tamanio);
		recv(socket_cliente, buffer, tamanio, MSG_WAITALL);
		log_info(mi_log, string_from_format("Se recibio el mensaje: %s\n", (char*) buffer));

		t_mensaje* mensaje = crear_mensaje(buffer,tamanio);

		agregar_msj_cola(queue,mensaje);
}

/*Por ahora solo crea una estrucutura t_mensaje con algunos valores, no todos*/
t_mensaje* crear_mensaje(void* buffer,int tamanio){

	t_mensaje* mensaje  = malloc(sizeof(t_mensaje));
	mensaje->buffer = malloc(sizeof(t_buffer));
	mensaje->id = asignar_id_univoco();
	mensaje->buffer->size = tamanio;
	mensaje->buffer->stream = malloc(mensaje->buffer->size);
	memcpy(mensaje->buffer->stream,buffer,mensaje->buffer->size);

	return mensaje;
}

void agregar_msj_cola(t_mq* queue,t_mensaje* mensaje){
	int tamanio_previo = queue_size(queue->cola);   //Esta solo para confirmar que que se agrego correctamente el msj a la cola

	queue_push(queue->cola,mensaje);
	if(queue_size(queue->cola) > tamanio_previo)
		log_info(mi_log,"Se agrego correctamente el mensaje a la cola");
}

void enviar_mensaje_suscriptores(t_mq* cola){
	t_mensaje* mensaje = queue_pop(cola->cola);
	int i;
	for(i=0;i<list_size(cola->suscriptores);i++){
		enviar_mensaje(mensaje,list_get(cola->suscriptores,i));
	}
	log_info(mi_log,"Se envio el mensaje a todos los suscriptores \n");

}

void enviar_mensaje(t_mensaje* mensaje, int cliente)
{

	t_paquete* paquete= malloc(sizeof(paquete));

	paquete->id = mensaje->id;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = mensaje->buffer->size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje->buffer->stream, paquete->buffer->size);

	int bytes = paquete->buffer->size + 3*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	if(send(cliente, a_enviar, bytes, 0) > 0)
		log_info(mi_log,"Se envio correctamente el mensaje\n");

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
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
	return 1;
}
