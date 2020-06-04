#include"utils.h"


//CONEXIONES
void* serializar_paquete(t_paquete* paquete, int bytes){
	void * a_enviar = malloc(bytes);
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
	agregar_movimientos_en_x(entrenador_listo);
	agregar_movimientos_en_y(entrenador_listo);
	agregar_accion(entrenador_listo, catch_pokemon, 1);

	cambiar_estado_entrenador(entrenador_listo, READY);
	list_add(entrenadores_ready, entrenador_listo);
	pthread_mutex_unlock(&lock_de_planificacion);
}

void agregar_movimientos_en_x(entrenador* entrenador_listo) {
	int diferencia_en_x = diferencia_en_x_del_entrenador_al_pokemon(entrenador_listo, entrenador_listo->pokemon_en_busqueda);
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

void agregar_movimientos_en_y(entrenador* entrenador_listo) {
	int diferencia_en_y = diferencia_en_y_del_entrenador_al_pokemon(entrenador_listo, entrenador_listo->pokemon_en_busqueda);
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

int diferencia_en_x_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon) {
	return (pokemon->posicion->posicion_x - entrenador->posicion->posicion_x);
}

int diferencia_en_y_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon) {
	return (pokemon->posicion->posicion_y - entrenador->posicion->posicion_y);
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
/*
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
	return entrenador->pokemons_objetivo == entrenador->pokemons_adquiridos;
}


int hay_deadlock(){
	return !(necesito_mas_pokemons() || list_all_satisfy(entrenadores,el_entrenador_cumplio_su_objetivo));
}


void salida_entrenador(entrenador* entrenador){
	if(el_entreador_cumplio_su_objetivo(entrenador)){
		cambiar_estado_entrenador(entrenador,EXIT);
	}
}

int el_entrenador_esta_block_deadlock(entrenador* entrenador){
	return entrenador->estado == BLOCK_DEADLOCK;
}

int cantidad_del_mismo_pokemon_por_entrenador(t_list* lista_de_pokemons,pokemon* pokemon){

	int cantidad = 0;

	for(int i=0;i<list_size(lista_de_pokemons);i++){
		if(pokemon == list_get(lista_de_pokemons,i)){
			cantidad ++;
		}
	}
	return cantidad;
}

int tiene_mas_cantidad_de_ese_pokemon(t_list* pokemons_adquiridos,t_list* pokemons_objetivo, pokemon* pokemon){
	return cantidad_del_mismo_pokemon_por_entrenador(pokemons_adquiridos,pokemon) > cantidad_del_mismo_pokemon_por_entrenador(pokemons_objetivo,pokemon);
}

pokemon* pokemon_que_me_sobra(entrenador* entrenador){

	for(int i=0;i<list_size(entrenador->pokemons_adquiridos);i++){
		if(tiene_mas_cantidad_de_ese_pokemon(entrenador->pokemons_adquiridos,entrenador->pokemons_objetivo,list_get(entrenador->pokemons_adquiridos,i))){
			return list_get(entrenador->pokemons_adquiridos,i);
		}
	}
	return NULL;
}

pokemon* pokemon_que_necesito(entrenador* entrenador){

	for(int i=0;i<list_size(entrenador->pokemons_objetivo);i++){
		if(!tiene_mas_cantidad_de_ese_pokemon(entrenador->pokemons_adquiridos,entrenador->pokemons_objetivo,list_get(entrenador->pokemons_adquiridos,i))){
			return list_get(entrenador->pokemons_objetivo,i);
		}
	}
	return NULL;

}

t_list* entrenadores_con_block_deadlock(){
	return list_filter(entrenadores,el_entrenador_esta_block_deadlock);
}

*/

//GET
void get_pokemon() {
	//ITERAR LOS POKEMONS DEL OBJETIVO PARA ENVIAR UN GET POR CADA POKEMON
}

void realizar_get(char* key, void* value) {
	int socket_get = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(socket_get == -1) {
		//ACCION POR DEFAULT, NO HACER NADA
		log_info(logger, "9. Se realizará el GET por DEFAULT debido a que la conexion con el broker fallo.");
		log_info(nuestro_log, "9. Se realizará el GET por DEFAULT debido a que la conexion con el broker fallo.");
	} else {
		//ACCION CON EL BROKER
		//TODO realizar el mandado del GET al broker y el recibo del ID para luego esperar en LOCALIZED
		t_buffer* buffer = malloc(sizeof(t_buffer));
		buffer->tamanio = 0;
		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->numero_de_modulo = NUMERO_MODULO;
		paquete->codigo_de_operacion = MENSAJE;
		paquete->codigo_de_accion = GET;
		paquete->buffer = buffer;


		int bytes = obtener_tamanio_de_paquete(paquete);
		void* a_enviar = serializar_paquete(paquete, bytes);

		if(send(socket_get, a_enviar, bytes ,0) > 0){
			log_info(nuestro_log, "Se realizo el envio de GET correctamente");
			int id_localized;

			if(recv(socket_get,&id_localized, sizeof(int), 0) > 0){
				log_info(nuestro_log, "Se recibio el ID para esperar LOCALIZED correctamente");
			}
			else {

			}
		}
		else{

			log_info(logger, "9. No se pudo realizar el envio del GET al broker, se realizará el GET por DEFAULT debido a que la conexion con el broker fallo.");
			log_info(nuestro_log, "9. No se pudo realizar el envio del GET al broker, se realizará el GET por DEFAULT debido a que la conexion con el broker fallo.");
		}

		free(a_enviar);
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}
}



//CATCH
void catch_pokemon(entrenador* entrenador) {
	int socket_catch = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(socket_catch == -1) {
		//ACCION POR DEFAULT
		log_info(logger, "9. Se realizará el CATCH por DEFAULT debido a que la conexion con el broker fallo.");
		log_info(logger, "9. Se realizará el CATCH por DEFAULT debido a que la conexion con el broker fallo.");
		manejar_la_captura_del_pokemon(entrenador);
	} else {
		//ACCION CON EL BROKER
		//TODO realizar el mandado del CATCH al broker y el recibo del ID para luego esperar en CAUGHT
	}
}

void manejar_la_captura_del_pokemon(entrenador* entrenador) {
	pokemon* pokemon_en_captura = entrenador->pokemon_en_busqueda;
	log_info(logger, string_from_format("3. Se realiza la captura del pokemon %s, en la posicion %d|%d exitosamente.", pokemon_en_captura->nombre, pokemon_en_captura->posicion->posicion_x, pokemon_en_captura->posicion->posicion_y));
	log_info(nuestro_log, string_from_format("3. Se realiza la captura del pokemon %s, en la posicion %d|%d exitosamente.", pokemon_en_captura->nombre, pokemon_en_captura->posicion->posicion_x, pokemon_en_captura->posicion->posicion_y));
	agregar_objetivo_a_objetivo_global(pokemon_en_captura->nombre);
	list_add(entrenador->pokemons_adquiridos, pokemon_en_captura->nombre);
	destruir_pokemon(pokemon_en_captura);

	cambiar_estado_entrenador(entrenador, BLOCK_READY);
}

void destruir_pokemon(pokemon* pokemon) {
	free(pokemon->posicion);
	free(pokemon);
}

