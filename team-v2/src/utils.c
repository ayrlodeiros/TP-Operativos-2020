#include"utils.h"


//CONEXIONES
t_paquete* crear_paquete(codigo_operacion cod_op, codigo_accion cod_acc, t_buffer* buffer) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->numero_de_modulo = NUMERO_MODULO;
	paquete->codigo_de_operacion = cod_op;
	paquete->codigo_de_accion = cod_acc;
	paquete->buffer = buffer;
	return paquete;
}
void* serializar_paquete(t_paquete* paquete, int tamanio_a_enviar){
	void * a_enviar = malloc(tamanio_a_enviar);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->numero_de_modulo),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,&(paquete->codigo_de_operacion),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,&(paquete->codigo_de_accion),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,&(paquete->buffer->tamanio),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,paquete->buffer->stream, paquete->buffer->tamanio);

	return a_enviar;
}

int obtener_tamanio_de_paquete(t_paquete* paquete) {
	return paquete->buffer->tamanio + 4*sizeof(int);
}

void destruir_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

int crear_conexion_como_cliente(char *ip, char* puerto) {
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1) {
		log_error(nuestro_log, string_from_format("Falla al conectarse en IP: %s y PUERTO: %s", ip, puerto));
		freeaddrinfo(server_info);
		return -1;
	} else {
		log_info(nuestro_log, string_from_format("Conexion exitosa en IP: %s y PUERTO: %s", ip, puerto));
		freeaddrinfo(server_info);
		return socket_cliente;
	}
}

void liberar_conexion(int socket) {
	close(socket);
}

int levantar_servidor(char* ip, char* puerto) {

	int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);

	for (p=servinfo; p != NULL; p = p->ai_next) {
		if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

	    if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
	        close(socket_servidor);
	        continue;
	    }
	    break;
	}

	log_info(nuestro_log, string_from_format("Servidor levantado en IP: %s y PUERTO: %s", ip, puerto));

	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);

	return socket_servidor;
}

void intentar_conectar_al_broker(int conexion) {
	char* ip_broker = leer_ip_broker();
	char* puerto_broker = leer_puerto_broker();

	while(conexion == -1) {
		log_info(nuestro_log, string_from_format("Intentando conectar con broker en IP: %s y PUERTO: %s", ip_broker, puerto_broker));
		log_info(logger, "10. Inicio de proceso de intento de comunicación con el Broker.");

		conexion = crear_conexion_como_cliente(ip_broker, puerto_broker);

		if(conexion == -1) {
			log_info(logger, "11. Intento de conexion con el broker fallida. Se volvera a intentar.");
			sleep(leer_tiempo_reconexion());
		} else {
			log_info(logger, "11. Intento de conexion con el broker exitosa.");
		}
	}
}

void suscribirse_a_cola_appeared() {
	int conexion_a_broker = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(conexion_a_broker == -1) {
		intentar_conectar_al_broker(conexion_a_broker);
	}
}

void suscribirse_a_cola_localized() {
	int conexion_a_broker = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(conexion_a_broker == -1) {
		intentar_conectar_al_broker(conexion_a_broker);
	}
}

void suscribirse_a_cola_caught() {
	int conexion_a_broker = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(conexion_a_broker == -1) {
		intentar_conectar_al_broker(conexion_a_broker);
	}
}

void levantar_conexiones_al_broker() {
	int primera_conexion = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(primera_conexion == -1) {
		funciona_broker = 0;

		intentar_conectar_al_broker(primera_conexion);

		funciona_broker = 1;
	}
	pthread_t* cola_appeared = pthread_create(&cola_appeared,NULL,suscribirse_a_cola_appeared, NULL);
	pthread_t* cola_localized = pthread_create(&cola_localized,NULL,suscribirse_a_cola_localized, NULL);
	pthread_t* cola_caught = pthread_create(&cola_caught,NULL,suscribirse_a_cola_caught, NULL);

	pthread_detach(cola_appeared);
	pthread_detach(cola_localized);
	pthread_detach(cola_caught);

}

//FIN DE CONEXIONES
//
//
//

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado){
	entrenador->estado = un_estado;
}

//Manejo la llegada de un nuevo pokemon (LOCALIZED O APPEARED)
void manejar_aparicion_de_pokemon(char* nombre, int posicion_x, int posicion_y) {
	if(el_pokemon_es_requerido(nombre)) {
		log_info(nuestro_log, string_from_format("Aparecio un %s, el cual es requerido", nombre));

		pokemon* nuevo_pokemon  = malloc(sizeof(pokemon));
		nuevo_pokemon->nombre = nombre;
		nuevo_pokemon->posicion = armar_posicion(string_from_format("%d|%d", posicion_x, posicion_y));

		buscar_entrenador_a_planificar_para_moverse(nuevo_pokemon);
	}
}

//Funcion que ejecuta el hilo cuando se le manda la señal de liberacion de un entrenador
void buscar_entrenador_disponible(){
	while(1){
		if(!list_any_satisfy(entrenadores, el_entrenador_se_puede_planificar) || queue_size(pokemons_sin_entrenador)==0) {
			log_info(nuestro_log, "Esperando a que algun entrenador se libere");
			pthread_mutex_lock(&lock_de_entrenador_disponible);
		}

		if(list_any_satisfy(entrenadores, el_entrenador_se_puede_planificar) && queue_size(pokemons_sin_entrenador)>0) {
			log_info(nuestro_log,"Se libero entrenador para buscar pokemon sin asignar");
			buscar_entrenador_a_planificar_para_moverse(queue_pop(pokemons_sin_entrenador));
		}
	}
}

void buscar_entrenador_a_planificar_para_moverse(pokemon* pokemon_objetivo){
	//Seteo la variable global del utils para poder manejarla en los distintos metodos que me filtran al entrenador mas cerca
	pokemon_para_planificar = pokemon_objetivo;
	//Filtro entrenadores en estado NEW o BLOCK_READY, luego ordeno la lista para obtener al primero mas cercano y despues lo agrego a la lista de entredores ready
	t_list* entrenadores_mas_cercanos = list_sorted(list_filter(entrenadores, el_entrenador_se_puede_planificar), el_entrenador1_esta_mas_cerca);
	if(list_size(entrenadores_mas_cercanos) == 0) {
		queue_push(pokemons_sin_entrenador, pokemon_objetivo);
		log_info(nuestro_log,"No hay entrenadores disponibles en este momento, cuando alguno finalice el hilo de entrenadores disponibles se encargara de la asignacion.");
	} else {
		log_info(nuestro_log, string_from_format("Se encontro entrenador para asignar a la busqueda de %s", pokemon_objetivo->nombre));
		agregar_entrenador_a_entrenadores_ready(list_get(entrenadores_mas_cercanos, 0), pokemon_objetivo);
	}
	list_destroy(entrenadores_mas_cercanos);
}

void agregar_entrenador_a_entrenadores_ready(entrenador* entrenador_listo, pokemon* pokemon_suelto) {

	entrenador_listo->pokemon_en_busqueda = pokemon_suelto;
	calcular_distancia_en_x_del_entrenador_a_la_posicion(entrenador_listo,entrenador_listo->pokemon_en_busqueda->posicion);
	calcular_distancia_en_y_del_entrenador_a_la_posicion(entrenador_listo,entrenador_listo->pokemon_en_busqueda->posicion);
	agregar_accion(entrenador_listo, catch_pokemon, 1);

	cambiar_estado_entrenador(entrenador_listo, READY);
	list_add(entrenadores_ready, entrenador_listo);
	pthread_mutex_unlock(&lock_de_planificacion);
}

void agregar_movimientos_en_x(entrenador* entrenador_listo, int diferencia_en_x) {
	if(diferencia_en_x > 0) {
		for(int i = 0; i < diferencia_en_x; i++) {
			agregar_accion(entrenador_listo, moverse_derecha, 1);
		}
	} else if(diferencia_en_x < 0) {
		for(int i = 0; i > diferencia_en_x; i--) {
			agregar_accion(entrenador_listo, moverse_izquierda, 1);
		}
	}
}

void agregar_movimientos_en_y(entrenador* entrenador_listo, int diferencia_en_y) {
	if(diferencia_en_y > 0) {
		for(int i = 0; i < diferencia_en_y; i++) {
			agregar_accion(entrenador_listo, moverse_arriba, 1);
		}
	} else if(diferencia_en_y < 0) {
		for(int i = 0; i > diferencia_en_y; i--) {
			agregar_accion(entrenador_listo, moverse_abajo, 1);
		}
	}
}

void agregar_accion(entrenador* entrenador_listo, void* movimiento, int cpu_requerido) {
	accion* accion_entrenador = armar_accion(movimiento, cpu_requerido);
	entrenador_listo->cpu_disponible += cpu_requerido;
	queue_push(entrenador_listo->acciones, accion_entrenador);
}


int el_entrenador_se_puede_planificar(entrenador* un_entrenador){
	return un_entrenador->estado == NEW || un_entrenador->estado == BLOCK_READY;
}

int el_entrenador1_esta_mas_cerca(entrenador* entrenador1, entrenador* entrenador2) {
	return distancia_del_entrenador_al_pokemon(entrenador1,pokemon_para_planificar) <= distancia_del_entrenador_al_pokemon(entrenador2,pokemon_para_planificar);
}

int distancia_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon) {
	return (int) (fabs(entrenador->posicion->posicion_x - pokemon->posicion->posicion_x)) + (int) (fabs(entrenador->posicion->posicion_y - pokemon->posicion->posicion_y));
}

void calcular_distancia_en_x_del_entrenador_a_la_posicion(entrenador* entrenador, posicion* posicion) {
	agregar_movimientos_en_x(entrenador,posicion->posicion_x - entrenador->posicion->posicion_x);
}

void calcular_distancia_en_y_del_entrenador_a_la_posicion(entrenador* entrenador, posicion* posicion) {
	agregar_movimientos_en_y(entrenador,posicion->posicion_y - entrenador->posicion->posicion_y);
}


int el_pokemon_es_requerido(char* nombre_pokemon){
	return dictionary_has_key(objetivo_global,nombre_pokemon)
			&& necesito_mas_de_ese_pokemon(nombre_pokemon);
}

//Si encuentra en el objetivo global, una cantidad mayor a 0 de ese pokemon, entonces lo necesitamos. Al devolver un numero mayor a 0 -> True
int necesito_mas_de_ese_pokemon(char* nombre_pokemon){
	return ((int) dictionary_get(objetivo_global,nombre_pokemon)) > 0;
}


//PARTE DE DETECCION DEADLOCK (FALTA TERMINAR)

void planear_intercambio(entrenador* entrenador1){
	log_info(nuestro_log,"Entre a planear el intercambio");

	cambiar_estado_entrenador(entrenador1, BLOCK);

	t_list* entrenadores_para_intercambio = obtener_entrenadores_para_intercambiar(entrenador1);

	if(list_size(entrenadores_para_intercambio) == 2){
		entrenador1->entrenador_en_busqueda = list_get(entrenadores_para_intercambio, 1);
		cambiar_estado_entrenador(entrenador1->entrenador_en_busqueda, BLOCK);
		log_info(nuestro_log,"Estoy en el principio de planear el intercambio");
		calcular_distancia_en_x_del_entrenador_a_la_posicion(entrenador1, entrenador1->entrenador_en_busqueda->posicion);
		calcular_distancia_en_y_del_entrenador_a_la_posicion(entrenador1, entrenador1->entrenador_en_busqueda->posicion);
		log_info(nuestro_log,"Estoy en el medio de planear el intercambio");
		agregar_accion(entrenador1, intercambiar, 5);
		cambiar_estado_entrenador(entrenador1, READY);
		list_add(entrenadores_ready, entrenador1);
	} else {
		cambiar_estado_entrenador(entrenador1, BLOCK_DEADLOCK);
	}
	log_info(nuestro_log,"Sali de planear el intercambio");

}

t_list* obtener_entrenadores_para_intercambiar(entrenador* entrenador1){
	int se_encontro_entrenador_ideal = 0;
	entrenador* entrenador_ideal;
	entrenador* entrenador_util;

	t_list* entrenadores_del_intercambio = list_create();
	list_add(entrenadores_del_intercambio, entrenador1);

	t_list* entrenadores_en_deadlock = entrenadores_con_block_deadlock();
	for(int i = 0; i < list_size(entrenadores_en_deadlock); i++){

		if(obtener_pokemons_que_necesito(entrenador1) == obtener_pokemons_que_me_sobran(list_get(entrenadores_en_deadlock,i))){

			if(obtener_pokemons_que_me_sobran(entrenador1) == obtener_pokemons_que_necesito(list_get(entrenadores_en_deadlock,i))){
				entrenador_ideal = list_get(entrenadores_en_deadlock,i);
				log_info(nuestro_log,string_from_format("HAY UN ENTRENADOR PARA INTERCAMBIAR PARTE 2"));
				se_encontro_entrenador_ideal = 1;
			}

			else{
				entrenador_util = list_get(entrenadores_en_deadlock,i);
			}
		}
	}

	if(se_encontro_entrenador_ideal == 1){
		log_info(nuestro_log,string_from_format("HAY UN ENTRENADOR PARA INTERCAMBIAR PARTE 3"));
		list_add(entrenadores_del_intercambio, entrenador_ideal);
		return entrenadores_del_intercambio;
	}
	else{
		list_add(entrenadores_del_intercambio, entrenador_util);
		return entrenadores_del_intercambio;
	}

	return entrenadores_del_intercambio;
}

t_list* obtener_pokemons_que_me_sobran(entrenador* entrenador){
	t_list* pokemons_que_me_sobran = list_create();

	dictionary_iterator(entrenador->)

	for(int i=0;i<list_size(entrenador->pokemons_adquiridos);i++){
		dictionary_iterator(entrenador->)

		if(tiene_mas_cantidad_de_ese_pokemon(entrenador->pokemons_adquiridos,entrenador->pokemons_objetivo,list_get(entrenador->pokemons_adquiridos,i))){
			return list_get(entrenador->pokemons_adquiridos,i);
		}
	}
	return NULL;
}

t_list* obtener_pokemons_que_necesito(entrenador* entrenador){
	t_list* pokemons_que_necesito = list_create();

	for(int i=0;i<list_size(entrenador->pokemons_objetivo);i++){
		if(!tiene_mas_cantidad_de_ese_pokemon(entrenador->pokemons_adquiridos, entrenador->pokemons_objetivo,list_get(entrenador->pokemons_adquiridos,i))){
			return list_get(entrenador->pokemons_objetivo,i);
		}
	}
	return NULL;

}

int necesito_mas_pokemons(){
	int i=0;

	while(i<dictionary_size(objetivo_global)){
		if((int)dictionary_get(objetivo_global,i) > 0){
			return 1;
		}
	}
	return 0;
}

int el_entrenador_cumplio_su_objetivo(entrenador* entrenador){


	int se_cumple = 1;

	if(list_size(entrenador->pokemons_adquiridos) == list_size(entrenador->pokemons_objetivo)){

		for(int i = 0; i<list_size(entrenador->pokemons_adquiridos);i++){
			printf("\n CANTIDAD DEL MISMO POKEMON ADQUIRIDOS: %d", cantidad_del_mismo_pokemon_por_entrenador(entrenador->pokemons_adquiridos,list_get(entrenador->pokemons_adquiridos,i)));
			printf("\n CANTIDAD DEL MISMO POKEMON OBJETIVO : %d", cantidad_del_mismo_pokemon_por_entrenador(entrenador->pokemons_objetivo,list_get(entrenador->pokemons_adquiridos,i)));
			if (cantidad_del_mismo_pokemon_por_entrenador(entrenador->pokemons_adquiridos,list_get(entrenador->pokemons_adquiridos,i)) != cantidad_del_mismo_pokemon_por_entrenador(entrenador->pokemons_objetivo,list_get(entrenador->pokemons_adquiridos,i))){
				se_cumple = 0;
			}
		}
	}
	else{
		se_cumple = 0;
	}

	return se_cumple;

}


int hay_deadlock(){
	return !(necesito_mas_pokemons() || list_all_satisfy(entrenadores,el_entrenador_cumplio_su_objetivo));
}


int el_entrenador_esta_block_deadlock(entrenador* entrenador){
	return entrenador->estado == BLOCK_DEADLOCK;
}


int tiene_mas_cantidad_de_ese_pokemon(t_dictionary* pokemons_adquiridos, t_dictionary* pokemons_objetivo, char* pokemon){
	return dictionary_get(pokemons_adquiridos,pokemon) > dictionary_get(pokemons_objetivo,pokemon);
}

int es_el_pokemon_buscado(entrenador* entrenador1, char* pokemon_a_eliminar){

	char* pokemon_que_no_necesito = pokemon_que_me_sobra(entrenador1);

	return pokemon_que_no_necesito == pokemon_a_eliminar;
}

int devolver_posicion_en_la_lista_del_pokemon(entrenador* entrenador, char* pokemon_a_buscar){
	for(int i = 0; i< list_size(entrenador->pokemons_adquiridos);i++){
		if(list_get(entrenador->pokemons_adquiridos, i) == pokemon_a_buscar){
			return i;
		}
	}
	return -1;
}


t_list* entrenadores_con_block_deadlock(){
	return list_filter(entrenadores,el_entrenador_esta_block_deadlock);
}

int el_entrenador_esta_en_exit(entrenador* entrenador){
	return entrenador->estado == EXIT;
}

int terminaron_todos_los_entrenadores(){
	list_all_satisfy(entrenadores,el_entrenador_esta_en_exit);
}

int el_entrenador_no_puede_capturar_mas_pokemons(entrenador* entrenador){
	return list_size(entrenador->pokemons_adquiridos) == list_size(entrenador->pokemons_objetivo);
}

//Realiza un intercambio entre dos entrenadores que estaban bloqueados
void intercambiar(entrenador* entrenador1) {
	log_info(nuestro_log,string_from_format("Empezando la accion de intercambio"));

	realizar_intercambio(entrenador1, entrenador1->entrenador_en_busqueda);

	log_info(nuestro_log,string_from_format("Terminando la accion de intercambio"));

	//entrenador* entrenador_que_me_intercambia = entrenador_para_intercambiar(entrenador);
	//Vamos a tener que ver que pokemons no necesita el entrenador1, que pokemons no necesita el entrenador2,...
	//... ver si alguno de los dos necesita lo que el otro no necesita y:
	//Si ambos necesitan lo que el otro no necesita: ambos van a recibir algo que necesitan
	//Si uno solo necesita lo que el otro no necesita: uno va a recibir lo que necesita y otro va a quedarse BLOCK
	//Si ninguno necesita lo que el otro no necesita: hacer el intercambio por cualquiera, pero quedarse esperando a que se BLOCKEE otro entrenador para intercambiar

	//Igual hay que ver bien como se va a trabajar a este metodo y cuando se lo va a llamar
	//Uno de los casos en el que se va a llamar es cuando un entrenador pase a BLOCK_DEADLOCK y ya haya alguno en ese estado tambien
}

void realizar_intercambio(entrenador* entrenador1, entrenador* entrenador_a_negociar){

	log_info(nuestro_log,string_from_format("Entrando a realizar Intercambio"));

	char* pokemon_que_requiero = pokemon_que_necesito(entrenador1);
	char* pokemon_que_no_necesito = pokemon_que_me_sobra(entrenador1);

	log_info(nuestro_log,string_from_format("Estoy en el medio de realizar Intercambio"));

	 //TODO REVISAR ESTO DE DEADLOCK
	if(devolver_posicion_en_la_lista_del_pokemon(entrenador1, pokemon_que_no_necesito) != -1){
		int posicion = devolver_posicion_en_la_lista_del_pokemon(entrenador1, pokemon_que_no_necesito);
		char* pokemon_a_intercambiar = list_remove(entrenador1->pokemons_adquiridos, posicion);
		list_add(entrenador_a_negociar->pokemons_adquiridos, pokemon_a_intercambiar);
	}

	if(devolver_posicion_en_la_lista_del_pokemon(entrenador_a_negociar,pokemon_que_no_necesito) != -1){
		int posicion = devolver_posicion_en_la_lista_del_pokemon(entrenador_a_negociar, pokemon_que_requiero);
		char* pokemon_a_intercambiar2 = list_remove(entrenador_a_negociar->pokemons_adquiridos, posicion);
		list_remove(entrenador_a_negociar->pokemons_adquiridos, posicion);
		list_add(entrenador1->pokemons_adquiridos, pokemon_a_intercambiar2);
	}

	log_info(nuestro_log,string_from_format("Estoy en el medio parte 2 de realizar Intercambio"));

	salida_entrenador(entrenador1);
	salida_entrenador(entrenador_a_negociar);
}

// FIN PARTE DEADLOCK
//
//


//ACCIONES DE TIPO MENSAJE
//GET
void get_pokemon() {
	//ITERAR LOS POKEMONS DEL OBJETIVO PARA ENVIAR UN GET POR CADA POKEMON
	dictionary_iterator(objetivo_global, realizar_get);
}

void realizar_get(char* key, void* value) {
	if(value > 0) {
		int socket_get = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
		if(socket_get == -1) {
			//ACCION POR DEFAULT, NO HACER NADA
			log_info(logger, "9. Se realizará el GET por DEFAULT debido a que la conexion con el broker fallo.");
			log_info(nuestro_log, "9. Se realizará el GET por DEFAULT debido a que la conexion con el broker fallo.");
		} else {
			//ACCION CON EL BROKER

			//CREO EL BUFFER CON SU TAMANIO Y STREAM
			t_buffer* buffer = malloc(sizeof(t_buffer));
			buffer->tamanio = strlen(key) + 1;
			void* stream = malloc(buffer->tamanio);
			int offset = 0;
			memcpy(stream + offset,&key, buffer->tamanio);
			buffer->stream = stream;

			//CREO EL PAQUETE CON EL CONTENIDO DE LO QUE VOY A ENVIAR
			t_paquete* paquete = crear_paquete(MENSAJE, GET, buffer);

			//SERIALIZO EL PAQUETE, LO MANDO y ESPERO LA RESPUESTA DEL BROKER
			int bytes = obtener_tamanio_de_paquete(paquete);
			void* a_enviar = serializar_paquete(paquete, bytes);

			if(send(socket_get, a_enviar, bytes ,0) > 0){
				log_info(nuestro_log, "Se realizo el envio de GET correctamente");

				pthread_t* hilo_espera_get;
				pthread_create(&hilo_espera_get,NULL, esperar_id_localized, socket_get);
				pthread_detach(hilo_espera_get);
			}
			else{

				log_info(logger, "9. No se pudo realizar el envio del GET al broker, se realizará el GET por DEFAULT debido a que la conexion con el broker fallo.");
				log_info(nuestro_log, "9. No se pudo realizar el envio del GET al broker, se realizará el GET por DEFAULT debido a que la conexion con el broker fallo.");
			}

			free(a_enviar);
			destruir_paquete(paquete);
		}
	}
}

void esperar_id_localized(int socket_get) {
	int id_localized;

	if(recv(socket_get,&id_localized, sizeof(int), 0) > 0){
		log_info(nuestro_log, string_from_format("Se recibio correctamente el ID: %d, para esperar en LOCALIZED", id_localized));

		pthread_mutex_lock(&mutex_lista_ids_localized);
		list_add(lista_ids_localized, &id_localized);
		pthread_mutex_unlock(&mutex_lista_ids_localized);
	} else {
		log_info(nuestro_log, "No se pudo recibir el ID de LOCALIZED");
	}

	liberar_conexion(socket_get);
}



//CATCH
void catch_pokemon(entrenador* entrenador) {
	cambiar_estado_entrenador(entrenador, BLOCK_READY);
	int socket_catch = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(socket_catch == -1) {
		//ACCION POR DEFAULT
		log_info(logger, "9. Se realizará el CATCH por DEFAULT debido a que la conexion con el broker fallo.");
		log_info(logger, "9. Se realizará el CATCH por DEFAULT debido a que la conexion con el broker fallo.");
		manejar_la_captura_del_pokemon(entrenador);
	} else {
		//ACCION CON EL BROKER

		//CREO EL BUFFER CON SU TAMANIO Y STREAM
		t_buffer* buffer = malloc(sizeof(t_buffer));

		int tamanio_char_pokemon = strlen(entrenador->pokemon_en_busqueda->nombre) + 1;
		buffer->tamanio = tamanio_char_pokemon + (2*sizeof(int));

		void* stream = malloc(buffer->tamanio);
		int offset = 0;
		memcpy(stream + offset,&(entrenador->pokemon_en_busqueda->nombre), tamanio_char_pokemon);
		offset += tamanio_char_pokemon;
		memcpy(stream + offset,&(entrenador->pokemon_en_busqueda->posicion->posicion_x), sizeof(int));
		offset += sizeof(int);
		memcpy(stream + offset,&(entrenador->pokemon_en_busqueda->posicion->posicion_y), sizeof(int));

		buffer->stream = stream;

		//CREO EL PAQUETE CON EL CONTENIDO DE LO QUE VOY A ENVIAR
		t_paquete* paquete = crear_paquete(MENSAJE, CATCH, buffer);

		//SERIALIZO EL PAQUETE, LO MANDO y ESPERO LA RESPUESTA DEL BROKER
		int bytes = obtener_tamanio_de_paquete(paquete);
		void* a_enviar = serializar_paquete(paquete, bytes);

		if(send(socket_catch, a_enviar, bytes ,0) > 0){
			log_info(nuestro_log, "Se realizo el envio de CATCH correctamente");

			pthread_t* hilo_espera_catch;
			pthread_create(&hilo_espera_catch,NULL, esperar_id_caught, socket_catch);
			pthread_detach(hilo_espera_catch);
		} else{
			log_info(logger, "9. No se pudo realizar el envio del CATCH al broker, se realizará el CATCH por DEFAULT debido a que la conexion con el broker fallo.");
			log_info(nuestro_log, "9. No se pudo realizar el envio del CATCH al broker, se realizará el CATCH por DEFAULT debido a que la conexion con el broker fallo.");
			manejar_la_captura_del_pokemon(entrenador);
		}

		free(a_enviar);
		destruir_paquete(paquete);
	}
}

void esperar_id_caught(int socket_catch) {
	int id_caught;

	if(recv(socket_catch,&id_caught, sizeof(int), 0) > 0){
		log_info(nuestro_log, string_from_format("Se recibio correctamente el ID: %d, para esperar en CAUGHT", id_caught));

		pthread_mutex_lock(&mutex_lista_ids_caught);
		list_add(lista_ids_caught, &id_caught);
		pthread_mutex_unlock(&mutex_lista_ids_caught);
	} else {
		log_info(nuestro_log, "No se pudo recibir el ID de CAUGHT");
	}
	liberar_conexion(socket_catch);
}

void manejar_la_captura_del_pokemon(entrenador* entrenador) {
	pokemon* pokemon_en_captura = entrenador->pokemon_en_busqueda;
	log_info(logger, string_from_format("3. Se realiza la captura del pokemon %s, en la posicion %d|%d exitosamente.", pokemon_en_captura->nombre, pokemon_en_captura->posicion->posicion_x, pokemon_en_captura->posicion->posicion_y));
	log_info(nuestro_log, string_from_format("3. Se realiza la captura del pokemon %s, en la posicion %d|%d exitosamente.", pokemon_en_captura->nombre, pokemon_en_captura->posicion->posicion_x, pokemon_en_captura->posicion->posicion_y));
	agregar_objetivo_a_objetivo_global(pokemon_en_captura->nombre, (void*) 1);
	agregar_pokemon_a_adquirido(entrenador->pokemons_adquiridos, pokemon_en_captura->nombre);
	destruir_pokemon(pokemon_en_captura);


	if(el_entrenador_cumplio_su_objetivo(entrenador)) {
		cambiar_estado_entrenador(entrenador, EXIT);
	} else if (el_entrenador_no_puede_capturar_mas_pokemons(entrenador)) {
		cambiar_estado_entrenador(entrenador, BLOCK_DEADLOCK);
		// Esto lo hice para que el entrenador no tenga la accion de intercambiar y no halla ningun otro entrenador para intercambiar ,y por eso pierda la accion
		if(list_size(entrenadores_con_block_deadlock()) >= 2 ){
			planear_intercambio(entrenador);
		}

	} else {
		cambiar_estado_entrenador(entrenador, READY);
		list_add(entrenadores_ready, entrenador);
	}
}

void agregar_pokemon_a_adquirido(t_dictionary* pokemons_adquiridos_entrenador, char* pokemon_adquirido) {
	if(dictionary_has_key(pokemons_adquiridos_entrenador, pokemon_adquirido)) {
		dictionary_put(pokemons_adquiridos_entrenador, pokemon_adquirido, dictionary_get(pokemons_adquiridos_entrenador, pokemon_adquirido)+1);
	} else {
		dictionary_put(pokemons_adquiridos_entrenador, pokemon_adquirido, 1);
	}
}

void destruir_pokemon(pokemon* pokemon) {
	free(pokemon->posicion);
	free(pokemon);
}
