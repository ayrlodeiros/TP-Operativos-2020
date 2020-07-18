#include "constructor.h"

void iniciar_funcionalidades() {
	inicializar_semaforos();
	inicializar_message_queues();
	iniciar_memoria_principal();
	iniciar_contador_ids_mensaje();
	iniciar_list_global();
	iniciar_signal_handler();
	iniciar_sigint_handler();
}

void int_handler(int signal){
	terminar_broker();
}

void iniciar_sigint_handler(void){
	signal(SIGINT, int_handler);

}

void iniciar_signal_handler() {
	if(signal(SIGUSR1, signal_handler) == SIG_ERR) {
		log_info(mi_log, "No se pudo recibir la SIGURS1");
	}
}

t_mensaje* obtener_mensaje_asociado(int inicio){
	for(int i = 0; list_size(lista_global_msjs) ;i++ ){
		t_mensaje* aux = list_get(lista_global_msjs,i);

		if(aux->pos_en_memoria->pos == inicio){
			return aux;
		}
	}
	log_info(mi_log,"ERROR NO SE PUDO IDENTIFICAR EL MENSAJE CON LA PARTICION");
	return NULL;
}


void dump_particion(int posicion,int inicio, int fin,bool libre,uint64_t lru){

	char estado;
	if(libre){
		estado = 'L';
		log_info(dump,"Particion %d: %p - %p.		[%c]	Size: %db",posicion,memoria_principal+inicio,memoria_principal+fin,estado,fin - inicio + 1);
	}
	else{
		//todo no probamos esto todavia
		estado = 'X';
		char lru_c[21];
		sprintf(lru_c, "%" PRIu64, lru);
		t_mensaje* mensaje = obtener_mensaje_asociado(inicio);
		log_info(dump,"Particion %d: %p - %p.		[%c]	Size: %db		LRU: %s		COLA: %d	ID: %d",posicion,memoria_principal+inicio,memoria_principal+fin,estado,fin - inicio + 1,lru_c,mensaje->cola,mensaje->id);
	}
}

void dump_solo_particion(void){

	for(int i = 0; list_size(lista_particiones) > i  ; i++){
			t_particion_dinamica* particion = list_get(lista_particiones,i);
			dump_particion(i,particion->inicio,particion->fin,particion->libre,particion->ult_vez_usado);
	}
}

void signal_handler(int signo) {
	log_info(dump,"------------------------------------------------------");
	char* fecha = obtener_fecha();
	log_info(dump,"Dump: %s", fecha);

	switch(leer_algoritmo_memoria()){
		case BS:
			for(int i = 0; list_size(lista_particiones) > i  ; i++){

				t_particion_bs* particion = list_get(lista_particiones,i);
				dump_particion(i,particion->inicio,particion->fin,particion->libre,particion->ult_vez_usado);

			}

			break;
		case PARTICIONES:
			for(int i = 0; list_size(lista_particiones) > i  ; i++){
				t_particion_dinamica* particion = list_get(lista_particiones,i);
				dump_particion(i,particion->inicio,particion->fin,particion->libre,particion->ult_vez_usado);
			}
			break;
		case NORMAL:
			break;
	}

	free(fecha);
	log_info(dump,"------------------------------------------------------");
	log_info(mi_log,"Se solicito un Dump de cache");
}

char* obtener_fecha() {
	time_t t;
	struct tm* tm;
	char* fecha_y_hora = malloc(20*sizeof(char));
	t=time(NULL);
	tm=localtime(&t);
	strftime(fecha_y_hora, 100, "%d/%m/%Y %H:%M:%S", tm);

	return fecha_y_hora;
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

	log_info(mi_log,"Message queues creadas correctamente ");

}

void iniciar_list_global(){
	lista_global_msjs = list_create();
}

void esperar_mensaje_en_cola(t_mq* mq) {

	while(1) {
		if(list_is_empty(mq->cola)) {
			log_info(mi_log, "Se bloquea la cola %d", mq->nombre);
			pthread_mutex_lock(&mq->lock);
		}

		log_info(mi_log, "Se desbloqueo la cola %d por la aparicion de un mensaje", mq->nombre);

		if(!list_is_empty(mq->cola)) {
			pthread_mutex_lock(&mutex_memoria_principal);
			enviar_mensaje_suscriptores(mq);
			pthread_mutex_unlock(&mutex_memoria_principal);
		}
	}
}

void enviar_mensaje(aux_msj_susc* msj_susc)
{
	pthread_t* hilo_ack;
	t_mensaje* mensaje = msj_susc->mensaje;
	suscriptor_t* suscriptor = msj_susc->suscriptor;
	t_paquete* paquete= malloc(sizeof(t_paquete));

	paquete->id = mensaje->id;
	paquete->id_cor = mensaje->id_cor;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = mensaje->pos_en_memoria->tamanio;
	paquete->buffer->stream = malloc(paquete->buffer->size);

	memcpy(paquete->buffer->stream, memoria_principal + mensaje->pos_en_memoria->pos, paquete->buffer->size);

	// TODO borrar al final
	if(mensaje->cola == CATCH) {
		int offset=0;
		int largo_nombre_pokemon;
		memcpy(&largo_nombre_pokemon, (paquete->buffer->stream) + offset, sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		log_info(mi_log, "El largo del nombre del pokemon es %d", largo_nombre_pokemon);
		char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
		int posicion_x;
		int posicion_y;
		memcpy(nombre_pokemon, (paquete->buffer->stream) + offset, largo_nombre_pokemon + 1);
		log_info(mi_log, "El nombre del pokemon es %s", nombre_pokemon);
		offset+=largo_nombre_pokemon + 1;
		memcpy(&posicion_x, (paquete->buffer->stream) + offset, sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		log_info(mi_log, "La posicion x del pokemon es %d", posicion_x);
		memcpy(&posicion_y, (paquete->buffer->stream) + offset, sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		log_info(mi_log, "La posicion y del pokemon es %d", posicion_y);
	}

	int bytes = paquete->buffer->size + 3*sizeof(uint32_t);

	void* a_enviar = serializar_paquete(paquete, bytes);

	if(send(suscriptor->conexion, a_enviar, bytes, 0) > 0){
		add_sub_lista_env_msj(mensaje,suscriptor);
		log_info(mi_log,"Se envio el mensaje al suscriptor de id %d y socket %d de la cola %d.",suscriptor->identificador,suscriptor->conexion, mensaje->cola);
		pthread_create(&hilo_ack, NULL, recibir_ACK ,msj_susc);
		pthread_detach(hilo_ack);
	}
	else
		log_info(mi_log,"NO se envio correctamente el mensaje al suscriptor.");

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

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

void recibir_ACK(aux_msj_susc* msj_y_susc){

	int valor;
	log_info(mi_log, "Estoy esperando acknowledgement del suscriptor %d.",msj_y_susc->suscriptor->identificador);
	if (recv(msj_y_susc->suscriptor->conexion, &valor, sizeof(int), MSG_WAITALL) < 0){

		log_info(mi_log,"No se recibio la confirmacion de envio del mensaje");

	}
	else{

		log_info(mi_log, "Se recibio el valor de ack: %d del suscriptor %d", valor, msj_y_susc->suscriptor->identificador);

		if(valor == 1) {
			log_info(mi_log, "Recibi la confirmacion de recepcion del suscriptor %d.",msj_y_susc->suscriptor->identificador);
			add_sub_lista_conf_msj(msj_y_susc->mensaje,msj_y_susc->suscriptor);
		} else {
			//TODO Ver como reaccionar en caso de que la recepcion no sea correcta
		}
	}

	free(msj_y_susc);
}

void add_sub_lista_conf_msj(t_mensaje* mensaje, suscriptor_t* suscriptor){
	list_add(mensaje->suscriptores_conf,suscriptor->identificador);
}

void add_sub_lista_env_msj(t_mensaje* mensaje,suscriptor_t* suscriptor){
	list_add(mensaje->suscriptores_env,suscriptor->identificador);
}

void enviar_mensaje_suscriptores(t_mq* cola){

	aux_msj_susc* aux = malloc(sizeof(aux_msj_susc));
	t_mensaje* mensaje = list_remove(cola->cola,0);
	suscriptor_t* suscriptor;
	int i;

	actualizar_ultima_vez_usado_particion(mensaje);

	for(i=0;i<list_size(cola->suscriptores);i++){
		suscriptor = list_get(cola->suscriptores,i);

		if(!msj_enviado_a_suscriptor(suscriptor->identificador,mensaje->suscriptores_conf)){

			aux->mensaje = mensaje;
			aux->suscriptor = suscriptor;
			enviar_mensaje(aux);

		}
	}
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
	get_mq->cola = list_create();
	get_mq->suscriptores = list_create();
	pthread_mutex_init(&get_mq->lock, NULL);
	pthread_mutex_lock(&get_mq->lock);
}

void crear_localized_mq(){
	localized_mq = malloc(sizeof(t_mq));
	localized_mq->nombre = LOCALIZED;
	localized_mq->cola = list_create();
	localized_mq->suscriptores = list_create();
	pthread_mutex_init(&localized_mq->lock, NULL);
	pthread_mutex_lock(&localized_mq->lock);
}

void crear_catch_mq(){
	catch_mq = malloc(sizeof(t_mq));
	catch_mq->nombre = CATCH;
	catch_mq->cola = list_create();
	catch_mq->suscriptores = list_create();
	pthread_mutex_init(&catch_mq->lock, NULL);
	pthread_mutex_lock(&catch_mq->lock);
}

void crear_caught_mq(){
	caught_mq = malloc(sizeof(t_mq));
	caught_mq->nombre = CAUGHT;
	caught_mq->cola = list_create();
	caught_mq->suscriptores = list_create();
	pthread_mutex_init(&caught_mq->lock, NULL);
	pthread_mutex_lock(&caught_mq->lock);
}
void crear_new_mq(){
	new_mq = malloc(sizeof(t_mq));
	new_mq->nombre = NEW;
	new_mq->cola = list_create();
	new_mq->suscriptores = list_create();
	pthread_mutex_init(&new_mq->lock, NULL);
	pthread_mutex_lock(&new_mq->lock);
}
void crear_appeared_mq(){
	appeared_mq = malloc(sizeof(t_mq));
	appeared_mq->nombre = APPEARED;
	appeared_mq->cola = list_create();
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
}

void liberar_mq(t_mq* mq){
	list_destroy(mq->suscriptores);
	//todo tener en cosideracion usar un list_destroy
	list_destroy(mq->cola);
	free(mq);
}

void liberar_get_mq(){
	list_destroy(get_mq->suscriptores);
	list_destroy(get_mq->cola);
	free(get_mq);
}

void liberar_localized_mq(){
	list_destroy(localized_mq->suscriptores);
	list_destroy(localized_mq->cola);
	free(localized_mq);
}
void liberar_catch_mq(){
	list_destroy(catch_mq->suscriptores);
	list_destroy(catch_mq->cola);
	free(catch_mq);
}
void liberar_caught_mq(){
	list_destroy(caught_mq->suscriptores);
	list_destroy(caught_mq->cola);
	free(caught_mq);
}
void liberar_new_mq(){
	list_destroy(new_mq->suscriptores);
	list_destroy(new_mq->cola);
	free(new_mq);
}
void liberar_appeared_mq(){
	list_destroy(appeared_mq->suscriptores);
	list_destroy(appeared_mq->cola);
	free(appeared_mq);
}

t_mensaje* crear_mensaje(void* buffer,int tamanio,mq_nombre cola,int id_correlativo){


	t_mensaje* mensaje  = malloc(sizeof(t_mensaje));

	int posicion = guardar_mensaje_en_memoria(tamanio, buffer);
	mensaje->id = asignar_id_univoco();
	log_info(mi_log, "El id del mensaje creado es %d", mensaje->id);
	mensaje->id_cor = id_correlativo;
	mensaje->cola = cola;
	mensaje->suscriptores_env = list_create();
	mensaje->suscriptores_conf = list_create();

	mensaje->pos_en_memoria = malloc(sizeof(t_pos_memoria));
	mensaje->pos_en_memoria->pos = posicion;
	mensaje->pos_en_memoria->tamanio = tamanio;
	log_info(mi_log,"SE GUARDO UN NUEVO MENSAJE EN LA COLA %d, POSICION %d y TAMANIO %d .",cola,posicion,tamanio);


	return mensaje;
}


/*Inicializa el contador de ids */
/* IDS MENSAJES */
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






/* ADMINISTRACION DE MEMORIA */

void iniciar_memoria_principal(){
	memoria_principal = malloc(leer_tamano_memoria());
	switch(leer_algoritmo_memoria()){
	case PARTICIONES:
		inicializar_lista_particiones();
		break;
	case BS:
		inicializar_lista_bs();
		break;
	case NORMAL:
		ultima_pos = 0;
		break;
	}

}

int guardar_mensaje_en_memoria(int tamanio, void* buffer){
	int posicion;

	pthread_mutex_lock(&mutex_memoria_principal);
	switch(leer_algoritmo_memoria()){
		case PARTICIONES:
			posicion = obtener_posicion_particiones(tamanio);
			almacenar_en_memoria(tamanio, buffer, posicion);
			break;
		case BS:
			posicion = obtener_posicion_bs(tamanio);
			almacenar_en_memoria(tamanio, buffer, posicion);
			break;
		case NORMAL:
			posicion = obtener_posicion_normal();
			almacenar_en_memoria(tamanio, buffer, posicion);
			break;
	}
	pthread_mutex_unlock(&mutex_memoria_principal);

	return posicion;
}

void almacenar_en_memoria(int tamanio, void* buffer, int posicion) {
	memcpy(memoria_principal+posicion, buffer, tamanio);
	//ultima_pos += tamanio;
}

int obtener_posicion_normal() {
	return ultima_pos;
}


//PARTICIONES DINAMICAS

void inicializar_lista_particiones(){
	t_particion_dinamica* primera_part = malloc(sizeof(t_particion_dinamica));
	primera_part->inicio = 0;
	primera_part->fin = leer_tamano_memoria() -1;
	primera_part->libre = true;
	lista_particiones = list_create();
	list_add(lista_particiones,primera_part);
}

int obtener_posicion_particiones(int tamanio) {

	int posicion = -1;
	int contador_compactacion = leer_frecuencia_compactacion();
	bool se_encontro_particion_libre = false;
	bool recien_se_compacto = false;


	while(!se_encontro_particion_libre){
		posicion = buscar_particion_libre(tamanio);

		if( posicion >= 0 ){
			se_encontro_particion_libre = true;

		}
		//el recien_se_compacto lo agrego para que en el caso que la frecuencia de compactacion sea cero, no se me quede en un loop infinito y nunca libere particiones
		else if (contador_compactacion == 0 && !recien_se_compacto)
		{
				log_info(mi_log,"SE PROCEDE A COMPACTAR LA MEMORIA.");
				compactacion();
				log_info(mi_log,"Se realizo la compactacion correctamente.");
				contador_compactacion = leer_frecuencia_compactacion();
				recien_se_compacto = true;
		}
			else {
				log_info(mi_log,"SE PROCEDE A LIBERAR MEMORIA");
				liberar_particion();
				contador_compactacion--;
				recien_se_compacto = false;
			}

	}
	return posicion;

}

bool estaOcupado(void* elemento){
	return !((t_particion_dinamica*)elemento)->libre;
}

bool noEstaOcupado(void* elemento){
	return ((t_particion_dinamica*)elemento)->libre;
}

void borrarParticion(void* elemento){
	log_info(mi_log,"Se ha eliminado una particion de memoria, su posicion de inicio era %d.",((t_particion_dinamica*) elemento)->inicio);
	free((t_particion_dinamica*) elemento);
}

// Lo dejo work in progress, basicamente para la compactacion para eliminar los particiones libres pensaba
/*bool libresPrimero(void* elemento,void* elementoDos){
	if( ((t_particion_dinamica*)elemento)->libre && !((t_particion_dinamica*)elementoDos)->libre ){
		return true;
	}
	if(){

}
*/

t_mensaje* obtener_estructura_msj(int posicion,t_list* lista_msjs){
	pthread_mutex_lock(&mutex_lista_msjs);
	log_info(mi_log,"La posicion que vas a buscar en la lista es: %d",posicion);
	for(int i = 0; list_size(lista_msjs) > i ;i++ ){

		t_mensaje* msj = list_get(lista_msjs,i);
		if(msj->pos_en_memoria->pos == posicion){

			return msj;
		}

	}
	pthread_mutex_lock(&mutex_lista_msjs);
	log_info(mi_log,"Retornas NULL");
	return NULL;

}

void actualizar_estructura_mensaje(int pos_vieja,int pos_nueva){
	log_info(mi_log,"La posicion vieja es : %d",pos_vieja);
	log_info(mi_log,"La posicion nueva es %d",pos_nueva);
	t_mensaje* mensaje = obtener_estructura_msj(pos_vieja,lista_global_msjs);
	log_info(mi_log,"El id del msj es : %d",mensaje->id);
	mensaje->pos_en_memoria->pos = pos_nueva;
	log_info(mi_log,"La nueva posicion inical del msj es : %d",mensaje->pos_en_memoria->pos);
}

void compactacion(){

	t_list* particiones_ocupadas = obtener_particiones_ocupadas();
	/* Hasta aca la lista de particiones, borre todas las estructuras que estaban libres y me quede con una nueva lista de particiones ocupadas */
	// esta medio a lo bestia, despues ver si se puede mejorar

	t_list* lista_temporal = crear_list_temporal(particiones_ocupadas);
	int prox_posicion = 0;
	int aux_para_encontrar_msjs;

	pthread_mutex_lock(&mutex_lista_msjs);
	for(int i = 0; list_size(lista_temporal) > i;i++){

		t_struct_temporal* aux = list_get(lista_temporal,i);
		t_particion_dinamica* particion = aux->particion;
		aux_para_encontrar_msjs = aux->particion->inicio;

		list_add(lista_particiones,particion);

		particion->inicio = prox_posicion;
		particion->fin = prox_posicion + particion->tamanio_ocupado -1;
		prox_posicion = prox_posicion + particion->tamanio_ocupado;
		log_info(mi_log,"La particion ocupada arranca en la posicion %d",particion->inicio);
		log_info(mi_log,"La particion ocupada terminar en la posicion %d",particion->fin);
		actualizar_estructura_mensaje(aux_para_encontrar_msjs,particion->inicio);

		llenar_memoria_principal(particion->inicio,particion->tamanio_ocupado,aux->memoria);

		free(aux->memoria);
		free(aux);

	}
	pthread_mutex_unlock(&mutex_lista_msjs);
	//Creo la particion libre y la agrego al final de la memoria
	list_destroy(lista_temporal);
	t_particion_dinamica* particion_libre = crear_particion_dinamica_libre();
	particion_libre->inicio = prox_posicion;
	particion_libre->fin = leer_tamano_memoria() - 1;
	list_add(lista_particiones,particion_libre);
	log_info(mi_log,"La particion libre que quedo arranca en la posicion %d",particion_libre->inicio);
	log_info(mi_log,"La particion libre que quedo arranca en la posicion %d",particion_libre->fin);

	}

void llenar_memoria_principal(int posicion, int tamanio, void* mensaje){
	memcpy(memoria_principal+posicion,mensaje,tamanio);
}

t_list* obtener_particiones_ocupadas(){
	t_list* particiones_ocupadas = list_filter(lista_particiones,estaOcupado);//filtrar lista de particiones llenas
	t_list* particiones_libres = list_filter(lista_particiones,noEstaOcupado);
	list_clean(lista_particiones);
	list_destroy_and_destroy_elements(particiones_libres,borrarParticion);
	return particiones_ocupadas;
}

t_list* crear_list_temporal(t_list* particiones){

	t_list* lista_temporal = list_create();

	for(int i = 0; list_size(particiones) > i;i++){
		t_struct_temporal* aux = malloc(sizeof(t_struct_temporal));
		aux->particion = list_get(particiones,i);
		aux->memoria = malloc(aux->particion->tamanio_ocupado);
		memcpy(aux->memoria,memoria_principal+aux->particion->inicio,aux->particion->tamanio_ocupado);
		list_add(lista_temporal,aux);
	}
	return lista_temporal;
}

void liberar_particion(){
	int ubicacion_particion;

	switch(leer_algoritmo_reemplazo()){
		case FIFO:
			ubicacion_particion = algoritmo_reemplazo_fifo();
			break;
		case LRU:
				ubicacion_particion = algoritmo_reemplazo_lru();
			break;
	}
	log_info(mi_log,"Se ha liberado la memoria en la particion %d",ubicacion_particion);
	consolidar(ubicacion_particion);
	log_info(mi_log,"Se termino de consolidar");
}

/* si hay tiempo, estas dos funciones son casi identicas, podrian abstraerse quedar mejor */
int algoritmo_reemplazo_fifo(void){

	t_particion_dinamica* primera_particion = NULL;
		int pos_primera_particion;

		for (int i = 0; list_size(lista_particiones) > i ;i++){
			t_particion_dinamica* particion = list_get(lista_particiones,i);

			if(!esta_libre(particion)){

				if(primera_particion == NULL || particion->tiempo_ingreso < primera_particion->tiempo_ingreso){
					primera_particion = particion;
					pos_primera_particion = i;
				}
			}
		}
		borrar_msj_mp(primera_particion->inicio);
		primera_particion->libre = true;
		primera_particion->tamanio_ocupado = 0;

		return pos_primera_particion;
}

int algoritmo_reemplazo_lru(void){

	t_particion_dinamica* part_menos_usada = NULL;
	int pos_part_menos_usada;

	for (int i = 0; list_size(lista_particiones) > i ;i++){
		t_particion_dinamica* particion = list_get(lista_particiones,i);

		log_info(mi_log,"La ultima vez usada la particion es :%d", particion->ult_vez_usado);

		if(!esta_libre(particion)){

			if(part_menos_usada == NULL || particion->ult_vez_usado < part_menos_usada->ult_vez_usado){

				part_menos_usada = particion;
				pos_part_menos_usada = i;

			}
		}
	}

	borrar_msj_mp(part_menos_usada->inicio);
	part_menos_usada->libre = true;
	part_menos_usada->tamanio_ocupado = 0;
	//consolidar() tal vez conviene mejor ponerlo aca
	return pos_part_menos_usada;
}

/* Supongo que funciona pero seguro se puede mejorar */
void consolidar(int pos_particion){
	log_info(mi_log,"CONSOLIDAR");
	t_particion_dinamica* liberada = list_get(lista_particiones,pos_particion);
	t_particion_dinamica* aux = liberada;
	int pos = pos_particion;

	if(particion_libre_a_la_izquierda(pos_particion,liberada->inicio)){
		log_info(mi_log,"Entra izquierda ");
		t_particion_dinamica* izquierda = list_get(lista_particiones,pos_particion-1);
		log_info(mi_log,"La particion de la izquierda arranca en %d",izquierda->inicio);
		log_info(mi_log,"La particion de la izquierda termina en %d",izquierda->fin);
		izquierda->fin = liberada->fin;
		aux = izquierda;
		list_remove_and_destroy_element(lista_particiones,pos,borrarParticion);
		pos = pos_particion -1;
		log_info(mi_log,"La particion auxiliar arranca en %d",aux->inicio);
		log_info(mi_log,"La particion auxiliar termina en %d",aux->fin);
	}
	if (particion_libre_a_la_derecha(pos,aux->fin)){
		log_info(mi_log,"Entra a la derecha");
		t_particion_dinamica* derecha = list_get(lista_particiones,pos+1);
		log_info(mi_log,"La particion aux arranca en %d",aux->inicio);
		log_info(mi_log,"La particion aux termina en %d",aux->fin);
		log_info(mi_log,"La particion de la izquierda arranca en %d",derecha->inicio);
		log_info(mi_log,"La particion de la izquierda termina en %d",derecha->fin);
		aux->fin = derecha->fin;
		list_remove_and_destroy_element(lista_particiones,pos+1,borrarParticion);
	}
}

bool particion_libre_a_la_izquierda(int posicion,int inicio_part_liberada){
	log_info(mi_log,"ME FIJO SI ESTA LIBRE A LA IZQUIERDA");
	return inicio_part_liberada - 1 >= 0 && esta_libre(list_get(lista_particiones,posicion-1));

}

bool particion_libre_a_la_derecha(int posicion,int fin_part_liberada){
	log_info(mi_log,"ME FIJO SI ESTA LIBRE A LA DERECHA");
	return fin_part_liberada + 1 <= leer_tamano_memoria()-1 && esta_libre(list_get(lista_particiones,posicion+1));
}


int buscar_particion_libre(int tamanio){

	switch(leer_algoritmo_particion_libre()){
		case FF:
			return algoritmo_first_fit(tamanio);
			break;
		case BF:
			return algoritmo_best_fit(tamanio);
			break;
		}
	return -1;
}

int algoritmo_best_fit(int tamanio){
	t_particion_dinamica* mejor_particion;
	int dif_mejor_part = -1;
	int posicion_mejor_part;


	for(int i = 0; list_size(lista_particiones) > i ; i++ ){
		t_particion_dinamica* particion_actual = list_get(lista_particiones,i);

		log_info(mi_log,"En best fit estoy analizando la particion cuyo inicio es %d:", particion_actual->inicio);

		if(esta_libre(particion_actual)){
			int diferencia = diferencia_tamanio_particion(particion_actual,tamanio);
			log_info(mi_log,"Esta libre esa particion %d:", particion_actual->inicio);
			log_info(mi_log,"La diferencica de tamanio entre esa particion y el mensaje es de %d:", diferencia);

			if(diferencia >= 0){

				if(diferencia < dif_mejor_part || dif_mejor_part < 0){
					mejor_particion = particion_actual;
					dif_mejor_part = diferencia;
					posicion_mejor_part = i;
				}
			}
		}
	}

	if(dif_mejor_part < 0){
		log_info(mi_log,"NO SE ENCONTRO PARTICION LIBRE");
		return -1;
	}
	else if(dif_mejor_part >= leer_tamano_minimo_particion())
		{
			return llenar_y_realizar_nueva_particion(mejor_particion,tamanio,posicion_mejor_part);
		}
		else
		{
			return llenar_particion(mejor_particion,tamanio);
		}

}

int algoritmo_first_fit(int tamanio){

	t_particion_dinamica* particion;

	for(int i = 0;list_size(lista_particiones) > i; i++){
		particion = list_get(lista_particiones,i);

		if(esta_libre(particion)){
			/* Esto es porque hay un minimo en el tamanio de particiones */
			int diferencia = diferencia_tamanio_particion(particion,tamanio);

			if(diferencia >= 0){

				if(diferencia >= leer_tamano_minimo_particion())
				{
						return llenar_y_realizar_nueva_particion(particion,tamanio,i);
				}
				else
				{
						return llenar_particion(particion,tamanio);
				}
			}
		}
	}
	/* No se encontro espacio libre para el msj */
	return -1;
}

t_particion_dinamica* crear_particion_dinamica_libre(){
	t_particion_dinamica* particion = malloc(sizeof(t_particion_dinamica));
	particion->libre = true;
	return particion;
}

int llenar_y_realizar_nueva_particion(t_particion_dinamica* particion,int tamanio,int posicion_en_lista){
	// no ovlidar que cuando elimino la particion se tiene que liberar este espacio de memoria
	t_particion_dinamica* nueva_particion = crear_particion_dinamica_libre();
	nueva_particion->fin = particion->fin;
	nueva_particion->inicio = particion->inicio + tamanio;


	// aunque tecnicamente no esta ocupado todavia no se me ocurre otro momento mejor para llenar este dato
	particion->tamanio_ocupado = tamanio;
	particion->fin = particion->inicio + tamanio - 1;
	particion->libre = false;
	particion->tiempo_ingreso = timestamp();
	particion->ult_vez_usado = timestamp();

	list_add_in_index(lista_particiones,posicion_en_lista+1,nueva_particion);

	return particion->inicio;

}
/* A diferencia del de arriba esto es cuando el msj tiene el mismo tamanio que la particion o un poco menos*/
int llenar_particion(t_particion_dinamica* particion, int tamanio){
	particion->tamanio_ocupado = tamanio;
	particion->libre = false;
	particion->tiempo_ingreso = timestamp();
	particion->ult_vez_usado = timestamp();
	return particion->inicio;
}

bool esta_libre(t_particion_dinamica* particion){
	return particion->libre;
}

int diferencia_tamanio_particion(t_particion_dinamica* particion,int tamanio_msj){
	return tamanio_particion(particion) - tamanio_msj;
}

int tamanio_particion(t_particion_dinamica* particion){
	return particion->fin - particion->inicio +1;
}


/* BUDDY SYSTEM */

void inicializar_lista_bs(){
	int tamanio_memoria = leer_tamano_memoria();

	t_particion_bs* primera_part = malloc(sizeof(t_particion_bs));
	primera_part->inicio = 0;
	primera_part->fin = tamanio_memoria-1;
	primera_part->potencia_de_dos = obtener_potencia_de_dos_mas_cercana(tamanio_memoria);
	primera_part->tiempo_ingreso = timestamp();
	primera_part->ult_vez_usado = timestamp();
	primera_part->libre = true;

	lista_particiones = list_create();
	list_add(lista_particiones, primera_part);

}

int obtener_posicion_bs(int tamanio) {

	int potencia_de_dos_limite = obtener_potencia_de_dos_mas_cercana(leer_tamano_minimo_particion());
	int potencia_de_dos_deseada = obtener_potencia_de_dos_mas_cercana(tamanio);
	//Esto se hace para respetar el tamaño minimo que puede tener una particion
	if(potencia_de_dos_limite > potencia_de_dos_deseada) {
		potencia_de_dos_deseada = potencia_de_dos_limite;
	}

	//OBTIENE LA POSICION DE LA PARTICION QUE MAS SE ACERCA A LA POTENCIA DE DOS DESEADA, DEVUELVE -1 SI NO ENCONTRO NIGUNA
	int posicion_de_particion_en_lista = obtener_posicion_particion_mas_cercana(potencia_de_dos_deseada);

	//REALIZAR LIBERACION Y CONSOLIDACION, HASTA OBTENER posicion DISTINTA DE -1
	while(posicion_de_particion_en_lista == -1) {
		liberar_y_consolidar();

		posicion_de_particion_en_lista = obtener_posicion_particion_mas_cercana(potencia_de_dos_deseada);
	}

	t_particion_bs* posible_particion = list_get(lista_particiones, posicion_de_particion_en_lista);

	//SI LA POTENCIA DE DOS DE LA PARTICION ENCONTRADA NO ES IGUAL A LA DESEADA SE PARTICIONA A LA PARTICION
	if(posible_particion->potencia_de_dos != potencia_de_dos_deseada) {
		// DIVIDIR EN DOS HASTA LLEGAR A POTENCIA DE DOS MAS CERCANA
		posible_particion = particionar_y_obtener_particion(posicion_de_particion_en_lista, potencia_de_dos_deseada);
	}

	// OCUPAR LA PARTICION
	posible_particion->libre = false;
	posible_particion->tiempo_ingreso = timestamp();
	posible_particion->ult_vez_usado = timestamp();

	return posible_particion->inicio;
}

int obtener_potencia_de_dos_mas_cercana(int valor) {
	double aux = valor;
	int cont = 0;
	if(valor < 2) {
		return 1;
	} else {
		while(aux > 1) {
			aux /= 2;
			cont ++;
		}
		return cont;
	}
}

int obtener_posicion_particion_mas_cercana(int potencia_de_dos) {
	int posicion = -1;

	for(int i = 0; i < list_size(lista_particiones); i++) {
		t_particion_bs* posible_particion = list_get(lista_particiones, i);
		if(posible_particion->libre) {
			if(posible_particion->potencia_de_dos == potencia_de_dos) {
				return i;
			}
			if(posible_particion->potencia_de_dos > potencia_de_dos) {
				posicion = i;
			}
		}
	}

	return posicion;
}

void liberar_y_consolidar() {
	int posicion_particion_liberada = liberar_una_particion();

	int posicion_consolidacion = evaluar_consolidacion(posicion_particion_liberada);
	//Hasta que no devuelva -1 quiere decir que se puede seguir intentando consolidar la misma particion
	while(posicion_consolidacion != -1) {
		posicion_consolidacion = evaluar_consolidacion(posicion_consolidacion);
	}
}

int liberar_una_particion() {
	int posicion_particion_liberada;

	switch(leer_algoritmo_reemplazo()){
		case FIFO:
			posicion_particion_liberada = obtener_posicion_de_particion_liberada_fifo();
			break;
		case LRU:
			posicion_particion_liberada = obtener_posicion_de_particion_liberada_lru();
			break;
	}

	return posicion_particion_liberada;
}

int obtener_posicion_de_particion_liberada_fifo() {
	t_particion_bs* particion_objetivo = NULL;
	int posicion;

	for(int i = 0; i<list_size(lista_particiones); i++) {
		t_particion_bs* particion_aux = list_get(lista_particiones, i);
		if(!(particion_aux->libre) && (particion_objetivo == NULL || particion_aux->tiempo_ingreso < particion_objetivo->tiempo_ingreso)) {
			particion_objetivo = particion_aux;
			posicion = i;
		}
	}

	borrar_msj_mp(particion_objetivo->inicio);
	particion_objetivo->libre = true;

	log_info(mi_log, "Se libero, por FIFO, la particion %d ubicada entre %p y %p", posicion, memoria_principal+(particion_objetivo->inicio), memoria_principal+(particion_objetivo->fin));

	return posicion;
}

int obtener_posicion_de_particion_liberada_lru() {
	t_particion_bs* particion_objetivo = NULL;
	int posicion;

	for(int i = 0; i<list_size(lista_particiones); i++) {
		t_particion_bs* particion_aux = list_get(lista_particiones, i);
		if(!(particion_aux->libre) && (particion_objetivo == NULL || particion_aux->ult_vez_usado < particion_objetivo->ult_vez_usado)) {
			particion_objetivo = particion_aux;
			posicion = i;
		}
	}

	borrar_msj_mp(particion_objetivo->inicio);
	particion_objetivo->libre = true;

	log_info(mi_log, "Se libero, por LRU, la particion %d ubicada entre %p y %p", posicion, memoria_principal+(particion_objetivo->inicio), memoria_principal+(particion_objetivo->fin));

	return posicion;
}

// EVALUA SI SE PUEDE CONSOLIDAR, Y CONSOLIDA DE SER POSIBLE, SI CONSOLIDO DEVUELVE LA POSICION EN LA QUE QUEDO LA PARTICION CONSOLIDADA, SI NO -1
int evaluar_consolidacion(int posicion_buddy_1) {
	int posicion_buddy_2;
	//CONSIGO LAS POSICIONES DE LOS DOS BUDDIES
	if(posicion_buddy_1%2 == 0) {
		posicion_buddy_2 = posicion_buddy_1 + 1;
	} else {
		posicion_buddy_2 = posicion_buddy_1 - 1;
	}

	t_particion_bs* buddy_2 = list_get(lista_particiones, posicion_buddy_2);

	if(buddy_2->libre) {
		int posicion_mas_chica;
		if(posicion_buddy_1 > posicion_buddy_2) {
			posicion_mas_chica = posicion_buddy_2;
			consolidar_buddies(posicion_buddy_1, buddy_2);
		} else {
			posicion_mas_chica = posicion_buddy_1;
			consolidar_buddies(posicion_buddy_2, list_get(lista_particiones, posicion_buddy_1));
		}

		return posicion_mas_chica;
	}

	return -1;
}

void consolidar_buddies(int posicion_buddy_a_eliminar, t_particion_bs* buddy_a_mantener) {
	t_particion_bs* buddy_eliminado = list_remove(lista_particiones, posicion_buddy_a_eliminar);


	buddy_a_mantener->potencia_de_dos++;
	buddy_a_mantener->fin = buddy_eliminado->fin;

	log_info(mi_log, "Se asociaron los bloques (buddies) con posiciones de inicio: %p y %p. Quedando una particion con valor de potencia de dos: %d", memoria_principal+(buddy_a_mantener->inicio), memoria_principal+(buddy_eliminado->inicio), buddy_a_mantener->potencia_de_dos);

	free(buddy_eliminado);
}

t_particion_bs* particionar_y_obtener_particion(int posicion_a_particionar, int potencia_de_dos_deseada) {
	t_particion_bs* particion_a_particionar = list_get(lista_particiones, posicion_a_particionar);

	int tamanio_actual = potencia(2, particion_a_particionar->potencia_de_dos);
	int tamanio_deseado = potencia(2, potencia_de_dos_deseada);

	t_list* lista_auxiliar = list_create();
	while(tamanio_actual != tamanio_deseado) {
		tamanio_actual /= 2;
		int potencia_nueva = obtener_potencia_de_dos_mas_cercana(tamanio_actual);

		t_particion_bs* nueva_particion = malloc(sizeof(t_particion_bs));
		nueva_particion->inicio = particion_a_particionar->inicio + tamanio_actual;
		nueva_particion->fin = nueva_particion->inicio + tamanio_actual -1;
		nueva_particion->potencia_de_dos = potencia_nueva;
		nueva_particion->libre = true;
		list_add(lista_auxiliar, nueva_particion);

		particion_a_particionar->fin = nueva_particion->inicio -1;
	}

	particion_a_particionar->potencia_de_dos = potencia_de_dos_deseada;

	// AGREGO LAS NUEVAS PARTICIONES A LA LISTA DE PARTICIONES, DETRAS DE LA PARTICION QUE CREE
	posicion_a_particionar++;
	for(int i = (list_size(lista_auxiliar)-1); i >= 0; i--) {
		list_add_in_index(lista_particiones, posicion_a_particionar, list_get(lista_auxiliar, i));
		posicion_a_particionar++;
	}


	list_destroy(lista_auxiliar);
	return particion_a_particionar;
}

int potencia(int base, int exponente) {
	int resultado = 1;
	for(int i = 0; i<exponente; i++) {
		resultado = resultado * base;
	}
	return resultado;
}



uint64_t timestamp(void) {

	struct timeval valor;
	gettimeofday(&valor, NULL);
	unsigned long long result = (((unsigned long long )valor.tv_sec) * 1000 + ((unsigned long) valor.tv_usec));
	uint64_t tiempo = result;

	return tiempo;

}

void borrar_msj_mp(int posicion){
	pthread_mutex_lock(&mutex_lista_msjs);
	for(int i=0; i< list_size(lista_global_msjs); i++) {
		t_mensaje* mensaje = list_get(lista_global_msjs, i);
		if(mensaje->pos_en_memoria->pos == posicion) {
			list_remove_and_destroy_element(lista_global_msjs, i, destruir_t_mensaje);
			break;
		}
	}
	pthread_mutex_unlock(&mutex_lista_msjs);
}

void chequear_y_borrar_msj_en_cola(t_mq* cola, int id_msj_borrado){

	for(int i = 0; list_size(cola->cola) > i;i++ ){

		t_mensaje* mensaje = list_get(cola->cola,i);
		if( mensaje->id == id_msj_borrado){
			list_remove(cola->cola,i);
			break;
		}

	}

}


void destruir_t_mensaje(t_mensaje* mensaje) {

	switch(mensaje->cola){
	case CATCH:
		chequear_y_borrar_msj_en_cola(catch_mq,mensaje->id);
		break;
	case CAUGHT:
		chequear_y_borrar_msj_en_cola(caught_mq,mensaje->id);
			break;
	case LOCALIZED:
		chequear_y_borrar_msj_en_cola(localized_mq,mensaje->id);
			break;
	case GET:
		chequear_y_borrar_msj_en_cola(get_mq,mensaje->id);
			break;
	case NEW:
		chequear_y_borrar_msj_en_cola(new_mq,mensaje->id);
			break;
	case APPEARED:
		chequear_y_borrar_msj_en_cola(appeared_mq,mensaje->id);
			break;
	}

	free(mensaje->pos_en_memoria);
	list_destroy(mensaje->suscriptores_conf);
	list_destroy(mensaje->suscriptores_env);
	free(mensaje);
}

void actualizar_ultima_vez_usado_particion(t_mensaje* mensaje) {

	//No van los mutex aca, porque cuando se llama a esta funcion, la memoria principal ya esta bloqueada
	switch(leer_algoritmo_memoria()){
		case PARTICIONES:
			actualizar_ultima_vez_dinamica(mensaje);
			break;
		case BS:
			actualizar_ultima_vez_bs(mensaje);
			break;
		case NORMAL:
			break;
	}
}

void actualizar_ultima_vez_dinamica(t_mensaje* mensaje) {
	for(int i=0; i< list_size(lista_particiones); i++) {
		t_particion_dinamica* particion = list_get(lista_particiones, i);
		if(mensaje->pos_en_memoria->pos == particion->inicio) {
			particion->ult_vez_usado = timestamp();
			break;
		}
	}
}

void actualizar_ultima_vez_bs(t_mensaje* mensaje) {
	for(int i=0; i< list_size(lista_particiones); i++) {
		t_particion_bs* particion = list_get(lista_particiones, i);
		if(mensaje->pos_en_memoria->pos == particion->inicio) {
			particion->ult_vez_usado = timestamp();
			break;
		}
	}
}
