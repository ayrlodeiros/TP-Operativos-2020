#include "protocolo.h"

void agregar_msj_cola(t_mq* queue,t_mensaje* mensaje){

	queue_push(queue->cola,mensaje);
	if(queue_size(queue->cola) > 0)
		printf("Se agrego correctamente el msj a la cola\n");


}
void agregar_suscriptor_cola(t_mq* cola,int suscriptor){
	list_add(cola->suscriptores,suscriptor);
	if(list_size(cola->suscriptores) > 0){
			printf("Se agrego el suscriptor correctamente\n");
			printf("El valor del suscriptor es: %d \n",list_get(cola->suscriptores,0));
	}
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void recibir_mensaje(int cliente, int* tamanio){
		t_mensaje* mensaje  = malloc(sizeof(t_mensaje));
		mensaje->contenido = malloc(sizeof(t_buffer));
		void* buffer;

		recv(cliente, tamanio, sizeof(int), MSG_WAITALL);
		buffer = malloc(*tamanio);
		recv(cliente, buffer, *tamanio, MSG_WAITALL);

		log_info(mi_log, string_from_format("Se recibio el mensaje: %s\n", (char*) buffer));

		mensaje->identificador = 1;
		mensaje->contenido->size = *tamanio;
		mensaje->contenido->stream = malloc(mensaje->contenido->size);
		memcpy(mensaje->contenido->stream,buffer,mensaje->contenido->size);

		agregar_suscriptor_cola(get_mq,cliente);
		agregar_msj_cola(get_mq,mensaje);
		enviar_mensaje_suscriptores(get_mq);
}


void devolver_mensaje(void* payload, int size, int socket_cliente)
{

	t_paquete* paquete = malloc(sizeof(paquete));


	paquete->codigo_operacion = GET;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	if(send(socket_cliente, a_enviar, bytes, 0) > 0)
		printf("Respuesta enviada al cliente correctamente\n");

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}


void enviar_mensaje_suscriptores(t_mq* cola){
	t_mensaje* mensaje = queue_pop(cola->cola);
	int i;
	for(i=0;i<list_size(cola->suscriptores);i++){
		devolver_mensaje(mensaje->contenido->stream,mensaje->contenido->size,list_get(cola->suscriptores,i));
		printf("Esto funco\n");
	}
	printf("Se envio el msj a todos los suscriptores\n");

}
