#include "constructor.h"

void iniciar_funcionalidades() {
	iniciar_config();
	inicializar_semaforos();
	inicializar_message_queues();
	iniciar_memoria_principal();
	iniciar_contador_ids_mensaje();
	iniciar_list_global();
	iniciar_signal_handler();
}

void iniciar_signal_handler() {
	if(signal(SIGUSR1, signal_handler) == SIG_ERR) {
		log_info(mi_log, "No se pudo recibir la SIGURS1");
	}
}

// TODO modificar en funcion de lo que dice el enunciado
void signal_handler(int signo) {
	log_info(mi_log,"Se recibio la SIGURS1");
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

void recibir_ACK(suscriptor_t* suscriptor,t_mensaje* mensaje){
	int valor;
	log_info(mi_log, "Estoy esperando acknowledgement del suscriptor %d.",suscriptor->identificador);
	if (recv(suscriptor->conexion, &valor, sizeof(int), MSG_WAITALL) < 0){
		log_info(mi_log,"No se recibio la confirmacion de envio del mensaje");
		/* Deberia actualizar el mensaje e indicar que no se recibio la confirmacion de recepcion*/
	}
	else{
		log_info(mi_log, "Se recibio el valor de ack: %d", valor);
		if(valor == 1) {
			log_info(mi_log, "Recibi el ack del suscriptor %d.",suscriptor->identificador);
			add_sub_lista_conf_msj(mensaje,suscriptor);
		} else {
			//TODO volver a mandar el mensaje, poner el mensaje en la cola
		}
	}
}

void add_sub_lista_conf_msj(t_mensaje* mensaje, suscriptor_t* suscriptor){
	list_add(mensaje->suscriptores_conf,suscriptor->identificador);
}

void add_sub_lista_env_msj(t_mensaje* mensaje,suscriptor_t* suscriptor){
	list_add(mensaje->suscriptores_env,suscriptor->identificador);
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
	ultima_pos += tamanio;
}

//todo tener en cuenta que al COMPACTAR se mueve la posicion del msj, por lo tanto hay que ver una forma de actualizar ese valor en la estructura msj

int obtener_posicion_normal() {
	return ultima_pos;
}


//PARTICIONES DINAMICAS

void inicializar_lista_particiones(){
	t_particion_dinamica* primera_part = malloc(sizeof(t_particion_dinamica));
	primera_part->inicio = 0;
	primera_part->fin = leer_tamano_memoria();
	primera_part->libre = true;
	lista_particiones_dinamicas = list_create();
	list_add(lista_particiones_dinamicas,primera_part);
}

int obtener_posicion_particiones(int tamanio) {

	int posicion;
	int contador_compactacion = leer_frecuencia_compactacion();
	bool se_encontro_particion_libre = false;
	bool recien_se_compacto = false;

	while(se_encontro_particion_libre){
		posicion = buscar_particion_libre(tamanio);

		if( posicion >= 0 ){
			se_encontro_particion_libre = true;
			//nose si es mejor directamente retornar resultado aca
		}
		//el recien_se_compacto lo agrego para que en el caso que la frecuencia de compactacion sea cero, no se me quede en un loop infinito y nunca libere particiones
		else if (contador_compactacion == 0 && !recien_se_compacto)
		{
				compactacion();
				contador_compactacion = leer_frecuencia_compactacion();
				recien_se_compacto = true;
		}
			else {
				liberar_particion();
				contador_compactacion--;
				recien_se_compacto = false;
			}

	}
	return posicion;

}

bool estaOcupado(void* elemento){
	return ((t_particion_dinamica*)elemento)->libre;
}

bool noEstaOcupado(void* elemento){
	return !((t_particion_dinamica*)elemento)->libre;
}

void borrarParticion(void* elemento){
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

void compactacion(){

	t_list* particiones_ocupadas = obtener_particiones_ocupadas();
	/* Hasta aca la lista de particiones, borre todas las estructuras que estaban libres y me quede con una nueva lista de particiones ocupadas */
	//todo esta medio a lo bestia, despues ver si se puede mejorar

	t_list* lista_temporal = crear_list_temporal(particiones_ocupadas);;
	int prox_posicion = 0;
	for(int i = 0; list_size(lista_temporal);i++){

		t_struct_temporal* aux = list_get(lista_temporal,i);
		t_particion_dinamica* particion = aux->particion;
		list_add(lista_particiones_dinamicas,particion);
		particion->inicio = prox_posicion;
		particion->fin = prox_posicion + particion->tamanio_ocupado -1;
		prox_posicion = prox_posicion + particion->tamanio_ocupado;
		/*todo acordarme de avisar a la estructura de msjs que se movieron los valores de la particion */
		llenar_memoria_principal(particion->inicio,particion->tamanio_ocupado,aux->memoria);
		free(aux->memoria);
		free(aux);

	}

	//Creo la particion libre y la agrego al final de la memoria
	list_destroy(lista_temporal);
	t_particion_dinamica* particion_libre = crear_particion_dinamica_libre();
	particion_libre->inicio = prox_posicion;
	particion_libre->fin = leer_tamano_memoria() - 1;
	list_add(lista_particiones_dinamicas,particion_libre);

	}

void llenar_memoria_principal(int posicion, int tamanio, void* mensaje){
	memcpy(memoria_principal+posicion,mensaje,tamanio);
}

t_list* obtener_particiones_ocupadas(){
	t_list* particiones_ocupadas = list_filter(lista_particiones_dinamicas,estaOcupado);//filtrar lista de particiones llenas
	t_list* particiones_libres = list_filter(lista_particiones_dinamicas,noEstaOcupado);
	list_clean(lista_particiones_dinamicas);
	list_destroy_and_destroy_elements(particiones_libres,borrarParticion);
	return particiones_ocupadas;
}

t_list* crear_list_temporal(t_list* particiones){

	t_list* lista_temporal = list_create();

	for(int i = 0; list_size(particiones);i++){
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
				algoritmo_reemplazo_lru();
			break;
	}
	consolidar(ubicacion_particion);
}

/*todo si hay tiempo, estas dos funciones son casi identicas, podrian abstraerse quedar mejor */
int algoritmo_reemplazo_fifo(void){

	t_particion_dinamica* primera_particion = NULL;
		int pos_primera_particion;

		for (int i = 0; list_size(lista_particiones_dinamicas) > 0 ;i++){
			t_particion_dinamica* particion = list_get(lista_particiones_dinamicas,i);

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

		//todo consolidar() tal vez conviene mejor ponerlo aca
		return pos_primera_particion;
}

int algoritmo_reemplazo_lru(void){

	t_particion_dinamica* part_menos_usada = NULL;
	int pos_part_menos_usada;

	for (int i = 0; list_size(lista_particiones_dinamicas) > 0 ;i++){
		t_particion_dinamica* particion = list_get(lista_particiones_dinamicas,i);

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
	//todo part_menos_usada->orden_ingreso;
	//todo consolidar() tal vez conviene mejor ponerlo aca
	return pos_part_menos_usada;
}

/* Supongo que funciona pero seguro se puede mejorar */
void consolidar(int pos_particion){

	t_particion_dinamica* liberada = list_get(lista_particiones_dinamicas,pos_particion);
	t_particion_dinamica* aux = liberada;
	int pos = pos_particion;

	if(particion_libre_a_la_izquierda(pos_particion)){
		t_particion_dinamica* izquierda = list_get(lista_particiones_dinamicas,pos_particion-1);
		izquierda->fin = liberada->fin;
		list_remove(lista_particiones_dinamicas,pos_particion);
		free(liberada);
		aux = izquierda;
		pos = pos_particion -1;
	}
	if (particion_libre_a_la_derecha(pos)){

		t_particion_dinamica* derecha = list_get(lista_particiones_dinamicas,pos+1);
		aux->inicio = derecha->inicio;
		list_remove(lista_particiones_dinamicas,pos+1);
		free(derecha);
	}
}

bool particion_libre_a_la_izquierda(int posicion){
	return posicion - 1 >= 0 && esta_libre(list_get(lista_particiones_dinamicas,posicion-1));

}

bool particion_libre_a_la_derecha(int posicion){
	return posicion + 1 <= leer_tamano_memoria()-1 && esta_libre(list_get(lista_particiones_dinamicas,posicion+1));
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

	for(int i = 0; list_size(lista_particiones_dinamicas) > i ; i++ ){
		t_particion_dinamica* particion_actual = list_get(lista_particiones_dinamicas,i);


		if(esta_libre(particion_actual)){
			int diferencia = diferencia_tamanio_particion(particion_actual,tamanio);

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

	for(int i = 0;list_size(lista_particiones_dinamicas) > i; i++){
		particion = list_get(lista_particiones_dinamicas,i);

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
	//todo no ovlidar que cuando elimino la particion se tiene que liberar este espacio de memoria
	t_particion_dinamica* nueva_particion = crear_particion_dinamica_libre();
	nueva_particion->fin = particion->fin;
	nueva_particion->inicio = nueva_particion->fin - tamanio + 1;


	//todo aunque tecnicamente no esta ocupado todavia no se me ocurre otro momento mejor para llenar este dato
	particion->tamanio_ocupado = tamanio;
	particion->fin = particion->inicio + tamanio - 1;
	particion->libre = false;
	particion->tiempo_ingreso = timestamp();
	particion->ult_vez_usado = timestamp();

	list_add_in_index(lista_particiones_dinamicas,posicion_en_lista+1,nueva_particion);

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

	lista_particiones_bs = list_create();
	t_particion_bs* primera_part = malloc(sizeof(t_particion_bs));
	primera_part->inicio = 0;
	primera_part->fin = tamanio_memoria; // Ver si usar el -1
	primera_part->potencia_de_dos = obtener_potencia_de_dos_mas_cercana(tamanio_memoria);
	primera_part->tiempo_ingreso = timestamp();
	primera_part->ult_vez_usado = timestamp();
	primera_part->libre = true;

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

	t_particion_bs* posible_particion = list_get(lista_particiones_bs, posicion_de_particion_en_lista);

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

	for(int i = 0; i < list_size(lista_particiones_bs); i++) {
		t_particion_bs* posible_particion = list_get(lista_particiones_bs, i);
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

	for(int i = 0; i<list_size(lista_particiones_bs); i++) {
		t_particion_bs* particion_aux = list_get(lista_particiones_bs, i);
		if(!(particion_aux->libre) && (particion_objetivo == NULL || particion_aux->tiempo_ingreso < particion_objetivo->tiempo_ingreso)) {
			particion_objetivo = particion_aux;
			posicion = i;
		}
	}

	borrar_msj_mp(particion_objetivo->inicio);
	particion_objetivo->libre = true;

	return particion_objetivo;
}

int obtener_posicion_de_particion_liberada_lru() {
	t_particion_bs* particion_objetivo = NULL;
	int posicion;

	for(int i = 0; i<list_size(lista_particiones_bs); i++) {
		t_particion_bs* particion_aux = list_get(lista_particiones_bs, i);
		if(!(particion_aux->libre) && (particion_objetivo == NULL || particion_aux->ult_vez_usado < particion_objetivo->ult_vez_usado)) {
			particion_objetivo = particion_aux;
			posicion = i;
		}
	}

	borrar_msj_mp(particion_objetivo->inicio);
	particion_objetivo->libre = true;

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

	t_particion_bs* buddy_2 = list_get(lista_particiones_bs, posicion_buddy_2);

	if(buddy_2->libre) {
		int posicion_mas_chica;
		if(posicion_buddy_1 > posicion_buddy_2) {
			posicion_mas_chica = posicion_buddy_2;
			consolidar_buddies(posicion_buddy_1, buddy_2);
		} else {
			posicion_mas_chica = posicion_buddy_1;
			consolidar_buddies(posicion_buddy_2, list_get(lista_particiones_bs, posicion_buddy_1));
		}

		return posicion_mas_chica;
	}

	return -1;
}

void consolidar_buddies(int posicion_buddy_a_eliminar, t_particion_bs* buddy_a_mantener) {
	t_particion_bs* buddy_eliminado = list_remove(lista_particiones_bs, posicion_buddy_a_eliminar);


	buddy_a_mantener->potencia_de_dos++;
	buddy_a_mantener->fin = buddy_eliminado->fin;


	free(buddy_eliminado);
}

t_particion_bs* particionar_y_obtener_particion(int posicion_a_particionar, int potencia_de_dos_deseada) {
	t_particion_bs* particion_a_particionar = list_get(lista_particiones_bs, posicion_a_particionar);

	int tamanio_actual = potencia(2, particion_a_particionar->potencia_de_dos);
	int tamanio_deseado = potencia(2, potencia_de_dos_deseada);

	t_list* lista_auxiliar = list_create();
	while(tamanio_actual != tamanio_deseado) {
		tamanio_actual /= 2;
		int potencia_nueva = obtener_potencia_de_dos_mas_cercana(tamanio_actual);

		t_particion_bs* nueva_particion = malloc(sizeof(t_particion_bs));
		nueva_particion->inicio = particion_a_particionar->inicio + tamanio_actual;
		nueva_particion->fin = nueva_particion->inicio + tamanio_actual; //si se usa el -1 cuando se crea, habria que usarlo aca tambien
		nueva_particion->potencia_de_dos = potencia_nueva;
		nueva_particion->libre = true;
		list_add(lista_auxiliar, nueva_particion);

		particion_a_particionar->fin = nueva_particion->inicio; //si se usa el -1 cuando se crea, habria que usarlo aca tambien
	}

	particion_a_particionar->potencia_de_dos = potencia_de_dos_deseada;

	// AGREGO LAS NUEVAS PARTICIONES A LA LISTA DE PARTICIONES, DETRAS DE LA PARTICION QUE CREE
	posicion_a_particionar++;
	for(int i = 0; i < list_size(lista_auxiliar); i++) {
		list_add_in_index(lista_particiones_bs, posicion_a_particionar+i, list_get(lista_auxiliar, i));
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

void destruir_t_mensaje(t_mensaje* mensaje) {
	free(mensaje->pos_en_memoria);
	list_destroy(mensaje->suscriptores_conf);
	list_destroy(mensaje->suscriptores_env);
	free(mensaje);
}
