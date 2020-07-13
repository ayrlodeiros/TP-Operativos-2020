#include "constructor.h"

void iniciar_funcionalidades() {
	iniciar_config();
	inicializar_semaforos();
	inicializar_message_queues();
	iniciar_memoria_principal();
	iniciar_contador_ids_mensaje();
	iniciar_list_global();
}

void inicializar_semaforos() {
	pthread_mutex_init(&mutex_id, NULL);
	pthread_mutex_init(&mutex_memoria_principal, NULL);
	pthread_mutex_init(&mutex_agregar_msj_a_cola, NULL);
	pthread_mutex_init(&mutex_lista_msjs,NULL);
}

void inicializar_message_queues(){

	crear_get_mq();
	crear_localized_mq();
	crear_catch_mq();
	crear_caught_mq();
	crear_new_mq();
	crear_appeared_mq();

	pthread_t* hilo_get;
	pthread_create(&hilo_get, NULL, esperar_mensaje_en_cola, get_mq);
	pthread_detach(hilo_get);

	pthread_t* hilo_localized;
	pthread_create(&hilo_localized, NULL, esperar_mensaje_en_cola, localized_mq);
	pthread_detach(hilo_localized);

	pthread_t* hilo_catch;
	pthread_create(&hilo_catch, NULL, esperar_mensaje_en_cola, catch_mq);
	pthread_detach(hilo_catch);

	pthread_t* hilo_caught;
	pthread_create(&hilo_caught, NULL, esperar_mensaje_en_cola, caught_mq);
	pthread_detach(hilo_caught);

	pthread_t* hilo_new;
	pthread_create(&hilo_new, NULL, esperar_mensaje_en_cola, new_mq);
	pthread_detach(hilo_new);

	pthread_t* hilo_appeared;
	pthread_create(&hilo_appeared, NULL, esperar_mensaje_en_cola, appeared_mq);
	pthread_detach(hilo_appeared);

	printf("Message queues creadas correctamente \n");

}

void iniciar_list_global(){
	lista_global_msjs = list_create();
}

void esperar_mensaje_en_cola(t_mq* mq) {

	while(1) {
		if(queue_is_empty(mq->cola)) {
			log_info(mi_log, "Se bloquea la cola %d", mq->nombre);
			pthread_mutex_lock(&mq->lock);
		}

		log_info(mi_log, "Se desbloqueo la cola %d por la aparicion de un mensaje", mq->nombre);

		enviar_mensaje_suscriptores(mq);
		//TODO implementar codigo cuando tenes mensaje para enviar de la cola
	}
}

void enviar_mensaje(aux_msj_susc* msj_susc)
{
	t_mensaje* mensaje = msj_susc->mensaje;
	suscriptor_t* suscriptor = msj_susc->suscriptor;
	t_paquete* paquete= malloc(sizeof(t_paquete));

	paquete->id = mensaje->id;
	paquete->id_cor = mensaje->id_cor;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = mensaje->pos_en_memoria->tamanio;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	//TODO abstraer en otra funcion la busqueda del stream en la memoria principal
	memcpy(paquete->buffer->stream, memoria_principal + mensaje->pos_en_memoria->pos, paquete->buffer->size);

	int bytes = paquete->buffer->size + 3*sizeof(uint32_t);

	void* a_enviar = serializar_paquete(paquete, bytes);

	if(send(suscriptor->conexion, a_enviar, bytes, 0) > 0){
		add_sub_lista_env_msj(mensaje,suscriptor);
		log_info(mi_log,"Se envio correctamente el mensaje al suscriptor de id %d y socket %d.",suscriptor->identificador,suscriptor->conexion);
	}
	else
		log_info(mi_log,"NO se envio correctamente el mensaje al suscriptor\n");

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	recibir_ACK(suscriptor,mensaje);
}

void enviar_mensaje_suscriptores(t_mq* cola){
	aux_msj_susc* aux = malloc(sizeof(aux_msj_susc));
	/*todo Por lo que estuve viendo en el foro, no es necesario que haya que sacar el msj de la cola, ya que puede que pasar que no lo hayan recibido todos los suscriptores */
	t_mensaje* mensaje = queue_pop(cola->cola);
	suscriptor_t* suscriptor;
	int i;

	for(i=0;i<list_size(cola->suscriptores);i++){
		suscriptor = list_get(cola->suscriptores,i);

		if(!msj_enviado_a_suscriptor(suscriptor->identificador,mensaje->suscriptores_conf)){
			//pthread_t* ack;
			aux->mensaje = mensaje;
			aux->suscriptor = suscriptor;
			/*pthread_create(&ack,NULL,(void*)enviar_mensaje,aux);
			pthread_detach(ack); */
			enviar_mensaje(aux);

		}
	}
	free(aux);
}

bool msj_enviado_a_suscriptor(int id_suscriptor,t_list* suscriptores_conf){
	int suscriptor_conf;
	for(int i=0;i<list_size(suscriptores_conf);i++){
		suscriptor_conf = (int) list_get(suscriptores_conf,i);
		if(id_suscriptor == suscriptor_conf) return true;
	}
	return false;
}


void crear_get_mq(){
	get_mq = malloc(sizeof(t_mq));
	get_mq->nombre = GET;
	get_mq->cola = queue_create();
	get_mq->suscriptores = list_create();
	pthread_mutex_init(&get_mq->lock, NULL);
	pthread_mutex_lock(&get_mq->lock);
}

void crear_localized_mq(){
	localized_mq = malloc(sizeof(t_mq));
	localized_mq->nombre = LOCALIZED;
	localized_mq->cola = queue_create();
	localized_mq->suscriptores = list_create();
	pthread_mutex_init(&localized_mq->lock, NULL);
	pthread_mutex_lock(&localized_mq->lock);
}

void crear_catch_mq(){
	catch_mq = malloc(sizeof(t_mq));
	catch_mq->nombre = CATCH;
	catch_mq->cola = queue_create();
	catch_mq->suscriptores = list_create();
	pthread_mutex_init(&catch_mq->lock, NULL);
	pthread_mutex_lock(&catch_mq->lock);
}

void crear_caught_mq(){
	caught_mq = malloc(sizeof(t_mq));
	caught_mq->nombre = CAUGHT;
	caught_mq->cola = queue_create();
	caught_mq->suscriptores = list_create();
	pthread_mutex_init(&caught_mq->lock, NULL);
	pthread_mutex_lock(&caught_mq->lock);
}
void crear_new_mq(){
	new_mq = malloc(sizeof(t_mq));
	new_mq->nombre = NEW;
	new_mq->cola = queue_create();
	new_mq->suscriptores = list_create();
	pthread_mutex_init(&new_mq->lock, NULL);
	pthread_mutex_lock(&new_mq->lock);
}
void crear_appeared_mq(){
	appeared_mq = malloc(sizeof(t_mq));
	appeared_mq->nombre = APPEARED;
	appeared_mq->cola = queue_create();
	appeared_mq->suscriptores = list_create();
	pthread_mutex_init(&appeared_mq->lock, NULL);
	pthread_mutex_lock(&appeared_mq->lock);
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

void liberar_mq(t_mq* mq){
	list_destroy(mq->suscriptores);
	queue_destroy(mq->cola);
	free(mq);
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

/*Por ahora solo crea una estrucutura t_mensaje con algunos valores, no todos*/
t_mensaje* crear_mensaje(void* buffer,int tamanio,mq_nombre cola,int id_correlativo){


	t_mensaje* mensaje  = malloc(sizeof(t_mensaje));

	int posicion = guardar_mensaje_en_memoria(tamanio, buffer);
	mensaje->id = asignar_id_univoco();
	mensaje->id_cor = id_correlativo;
	mensaje->cola = cola;
	mensaje->suscriptores_env = list_create();
	mensaje->suscriptores_conf = list_create();

	mensaje->pos_en_memoria = malloc(sizeof(t_pos_memoria));
	mensaje->pos_en_memoria->pos = posicion;
	mensaje->pos_en_memoria->tamanio = tamanio;
	log_info(mi_log,"Se guardo un nuevo mensaje en la cola %d, posicion %d y tamanio %d .",cola,posicion,tamanio);

	return mensaje;
}


/*Inicializa el contador de ids */
/*todo IDS MENSAJES */
void iniciar_contador_ids_mensaje(){
	contador_ids_mensaje = 1;
}

int asignar_id_univoco(){
	pthread_mutex_lock(&mutex_id);
	int valor = contador_ids_mensaje;
	contador_ids_mensaje++;
	pthread_mutex_unlock(&mutex_id);
	return valor;
}

void iniciar_memoria_principal(){
	memoria_principal = malloc(leer_tamano_memoria());
	ultima_pos = 0;
}

int guardar_mensaje_en_memoria(int tamanio, void* buffer){
	int posicion;

	switch(leer_algoritmo_memoria()){
		case PARTICIONES:
			posicion = obtener_posicion_particiones(tamanio, posicion);
			almacenar_en_memoria(tamanio, buffer, posicion);
			break;
		case BS:
			posicion = obtener_posicion_bs(tamanio, posicion);
			almacenar_en_memoria(tamanio, buffer, &posicion);
			break;
		case NORMAL:
			/*todo si hay error puede que sea q falta el * en posicion */
			posicion = obtener_posicion_normal();
			almacenar_en_memoria(tamanio, buffer, posicion);
			break;
	}
	return posicion;
}

void almacenar_en_memoria(int tamanio, void* buffer, int posicion) {
	pthread_mutex_lock(&mutex_memoria_principal);
	memcpy(memoria_principal+posicion, buffer, tamanio);
	ultima_pos += tamanio;
	pthread_mutex_unlock(&mutex_memoria_principal);
}

int obtener_posicion_particiones(int tamanio, int posicion) {
	//TODO impletar obtencion posicion particiones
	pthread_mutex_lock(&mutex_memoria_principal);



	pthread_mutex_unlock(&mutex_memoria_principal);
	return -1;
}

int obtener_posicion_bs(int tamanio, int posicion) {
	//TODO impletar obtencion posicion bs
	pthread_mutex_lock(&mutex_memoria_principal);



	pthread_mutex_unlock(&mutex_memoria_principal);
	return -1;
}

int obtener_posicion_normal() {
	return ultima_pos;
}

suscriptor_t* crear_suscriptor(int conexion_suscriptor,int id_modulo){
	suscriptor_t* suscriptor = malloc(sizeof(suscriptor));
	suscriptor->identificador = id_modulo;
	suscriptor->conexion = conexion_suscriptor;
	return suscriptor;
}

void agregar_a_lista_global(t_mensaje* mensaje){
	pthread_mutex_lock(&mutex_lista_msjs);
	list_add(lista_global_msjs,mensaje);
	pthread_mutex_unlock(&mutex_lista_msjs);
}
