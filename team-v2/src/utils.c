#include"utils.h"


//CONEXIONES
t_paquete* crear_paquete(codigo_operacion cod_op, codigo_accion cod_acc, t_buffer* buffer) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->numero_de_modulo = leer_id_modulo();
	paquete->codigo_de_operacion = cod_op;
	paquete->codigo_de_accion = cod_acc;
	paquete->buffer = buffer;
	return paquete;
}
t_paquete* crear_paquete_sin_buffer(codigo_operacion cod_op, codigo_accion cod_acc) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->numero_de_modulo = leer_id_modulo();
	paquete->codigo_de_operacion = cod_op;
	paquete->codigo_de_accion = cod_acc;
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

void* serializar_paquete_sin_buffer(t_paquete* paquete, int tamanio_a_enviar){
	void * a_enviar = malloc(tamanio_a_enviar);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->numero_de_modulo),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,&(paquete->codigo_de_operacion),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,&(paquete->codigo_de_accion),sizeof(int));

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
		//log_error(nuestro_log, string_from_format("Falla al conectarse en IP: %s y PUERTO: %s", ip, puerto));
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

	//PARA GAMEBOY
void atender_conexion_gameboy() {
	int servidor_para_gameboy = levantar_servidor("127.0.0.2", "5002");

	while (1) {
		struct sockaddr_in dir_cliente;

		int tam_direccion = sizeof(struct sockaddr_in);

		log_info(nuestro_log, "Listo para recibir conexion de GAMEBOY");
		int socket_cliente = accept(servidor_para_gameboy, (void*) &dir_cliente, &tam_direccion);

		log_info(nuestro_log, "SE CONECTO EL GAMEBOY");

		pthread_t* hilo_conexion_gameboy;
		pthread_create(&hilo_conexion_gameboy,NULL, recibir_mensaje_de_gameboy, socket_cliente);
		pthread_detach(hilo_conexion_gameboy);
	}
}

void recibir_mensaje_de_gameboy(int socket_gameboy) {
	int hubo_error = 0;

	int cod_modulo;
	if(recv(socket_gameboy, &cod_modulo, sizeof(int), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se puedo recibir el codigo de modulo");
	}
	codigo_operacion cod_op;
	if(recv(socket_gameboy, &cod_op, sizeof(codigo_operacion), MSG_WAITALL) == -1) {
		hubo_error = 1;;
		log_info(nuestro_log, "No se puedo recibir el codigo de operacion");
	}
	codigo_accion cod_acc;
	if(recv(socket_gameboy, &cod_acc, sizeof(codigo_accion), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se puedo recibir el codigo de accion");
	}

	int tamanio_stream;
	if(recv(socket_gameboy, &tamanio_stream, sizeof(int), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se puedo recibir el tamanio del stream");
	}

	int largo_nombre_pokemon;
	if(recv(socket_gameboy, &largo_nombre_pokemon, sizeof(int), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se puedo recibir el largo del nombre del pokemon");
	}

	char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
	int posicionX;
	int posicionY;
	if(recv(socket_gameboy, nombre_pokemon, largo_nombre_pokemon + 1, MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se puedo recibir el nombre del pokemon");
	}
	if(recv(socket_gameboy, &posicionX, sizeof(int), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se puedo recibir la posicion en X");
	}
	if(recv(socket_gameboy, &posicionY, sizeof(int), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se puedo recibir la posicion en Y");
	}

	if(hubo_error == 0) {
		manejar_aparicion_de_pokemon(nombre_pokemon, posicionX, posicionY);
	} else {
		log_info(nuestro_log, "No se pudo recibir el mensaje del Gameboy");
	}
}
	//FIN DE PARA GAMEBOY
	//
	//

	//PARA BROKER
int intentar_conectar_al_broker() {
	char* ip_broker = leer_ip_broker();
	char* puerto_broker = leer_puerto_broker();
	int conexion = -1;

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
	return conexion;
}

void levantar_conexiones_al_broker() {
	pthread_t* hilo_appeared;
	pthread_t* hilo_localized;
	pthread_t* hilo_caught;
	while(1) {
		if(funciona_broker == 1) {
			log_info(nuestro_log, "Funciona broker, activando las tres colas...");

			pthread_create(&hilo_appeared,NULL, esperar_mensaje_appeared, NULL);
			pthread_detach(hilo_appeared);
			pthread_create(&hilo_localized,NULL, esperar_mensaje_localized, NULL);
			pthread_detach(hilo_localized);
			pthread_create(&hilo_caught,NULL, esperar_mensaje_caught, NULL);
			pthread_detach(hilo_caught);

			log_info(nuestro_log, "Bloqueo de conexiones al broker");

			pthread_mutex_lock(&lock_reintento_broker);

			log_info(nuestro_log, "Desbloqueo de conexiones al broker");
		} else {

			log_info(nuestro_log, "El broker no esta conectado...");

			conexion_appeared = intentar_conectar_al_broker();

			conexion_localized = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());

			conexion_caught = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());

			if(conexion_appeared == -1 || conexion_localized == -1 || conexion_caught == -1) {
				cambiar_valor_de_funciona_broker(0);
			} else {
				cambiar_valor_de_funciona_broker(1);
			}
		}

	}
}

void esperar_mensaje_appeared() {
	suscribirse_a_cola(conexion_appeared, APPEARED);
	while(funciona_broker == 1) {
		log_info(nuestro_log, "Estoy esperando mensaje APPEARED");
		int asd;
		if(recv(conexion_appeared, &asd, sizeof(int), 0) > 0){

		} else {
			log_info(nuestro_log, "Se perdio la conexion con el broker");
			cambiar_valor_de_funciona_broker(0);
			desbloquear_lock_reintento();
		}
	}
}

void esperar_mensaje_localized() {
	suscribirse_a_cola(conexion_localized, LOCALIZED);
	while(funciona_broker == 1) {
		log_info(nuestro_log, "Estoy esperando mensaje LOCALIZED");
		int asd;
		if(recv(conexion_localized, &asd, sizeof(int), 0) > 0){

		} else {
			log_info(nuestro_log, "Se perdio la conexion con el broker");
			cambiar_valor_de_funciona_broker(0);
			desbloquear_lock_reintento();
		}
	}
}
void esperar_mensaje_caught() {
	suscribirse_a_cola(conexion_caught, CAUGHT);
	while(funciona_broker == 1) {
		log_info(nuestro_log, "Estoy esperando mensaje CAUGHT");
		int asd;
		if(recv(conexion_caught, &asd, sizeof(int), 0) > 0){

		} else {
			log_info(nuestro_log, "Se perdio la conexion con el broker");
			cambiar_valor_de_funciona_broker(0);
			desbloquear_lock_reintento();
		}
	}
}

void suscribirse_a_cola(int conexion_broker, codigo_accion cola_a_suscribir) {
	t_paquete* paquete = crear_paquete_sin_buffer(SUSCRIPCION, cola_a_suscribir);
	int tamanio_paquete = 3*sizeof(int);
	void* a_enviar = serializar_paquete_sin_buffer(paquete, tamanio_paquete);
	if(send(conexion_broker, a_enviar, tamanio_paquete, 0) > 0){
		log_info(nuestro_log, "Suscripcion exitosa a la cola");
	} else {
		log_info(nuestro_log, "No se pudo realizar la suscripcion, el broker no funciona");
		cambiar_valor_de_funciona_broker(0);
		desbloquear_lock_reintento();
	}
	free(paquete);
}

void cambiar_valor_de_funciona_broker(int new_value) {
	if(new_value != funciona_broker) {
		pthread_mutex_lock(&mutex_funciona_broker);
		funciona_broker = new_value;
		pthread_mutex_unlock(&mutex_funciona_broker);
	}
}

void desbloquear_lock_reintento() {
	if(funciona_broker == 0) {
		pthread_mutex_unlock(&lock_reintento_broker);
	}
}
	//FIN PARA BROKER
	//
	//

//FIN DE CONEXIONES
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
	} else {
		log_info(nuestro_log, string_from_format("El pokemon %s no es requerido", nombre));
	}
}

//Funcion que ejecuta el hilo cuando se le manda la señal de liberacion de un entrenador
void buscar_entrenador_disponible(){
	while(1){
		if(!list_any_satisfy(entrenadores, el_entrenador_se_puede_planificar) || queue_size(pokemons_sin_entrenador)==0) {
			log_info(nuestro_log, "Esperando alguna accion");
			pthread_mutex_lock(&lock_de_entrenador_disponible);
		}

		if(list_any_satisfy(entrenadores, el_entrenador_se_puede_planificar) && queue_size(pokemons_sin_entrenador)>0) {
			log_info(nuestro_log,"Se libero entrenador para buscar pokemon sin asignar");
			buscar_entrenador_a_planificar_para_moverse(queue_pop(pokemons_sin_entrenador));
		}
	}
}

void buscar_entrenador_a_planificar_para_moverse(pokemon* pokemon_objetivo){
	pthread_mutex_lock(&mutex_entrenadores);
	pthread_mutex_lock(&mutex_pokemon_para_planificar);
	//Seteo la variable global del utils para poder manejarla en los distintos metodos que me filtran al entrenador mas cerca
	pokemon_para_planificar = pokemon_objetivo;
	//Filtro entrenadores en estado NEW o BLOCK_READY, luego ordeno la lista para obtener al primero mas cercano y despues lo agrego a la lista de entredores ready
	t_list* entrenadores_mas_cercanos = list_sorted(list_filter(entrenadores, el_entrenador_se_puede_planificar), el_entrenador1_esta_mas_cerca);
	pthread_mutex_unlock(&mutex_pokemon_para_planificar);

	if(list_size(entrenadores_mas_cercanos) == 0) {
		queue_push(pokemons_sin_entrenador, pokemon_objetivo);
		log_info(nuestro_log,"No hay entrenadores disponibles en este momento, cuando alguno finalice el hilo de entrenadores disponibles se encargara de la asignacion.");
	} else {
		log_info(nuestro_log, string_from_format("Se encontro entrenador para asignar a la busqueda de %s", pokemon_objetivo->nombre));
		agregar_entrenador_a_entrenadores_ready(list_get(entrenadores_mas_cercanos, 0), pokemon_objetivo);
	}
	list_destroy(entrenadores_mas_cercanos);
	pthread_mutex_unlock(&mutex_entrenadores);
}

void agregar_entrenador_a_entrenadores_ready(entrenador* entrenador_listo, pokemon* pokemon_suelto) {

	entrenador_listo->pokemon_en_busqueda = pokemon_suelto;
	calcular_distancia_en_x_del_entrenador_a_la_posicion(entrenador_listo,entrenador_listo->pokemon_en_busqueda->posicion);
	calcular_distancia_en_y_del_entrenador_a_la_posicion(entrenador_listo,entrenador_listo->pokemon_en_busqueda->posicion);
	agregar_accion(entrenador_listo, catch_pokemon, 1);

	cambiar_estado_entrenador(entrenador_listo, READY);
	agregar_entrenador_a_lista_entrenadores_ready(entrenador_listo);
}

void agregar_entrenador_a_lista_entrenadores_ready(entrenador* entrenador_listo) {
	pthread_mutex_lock(&mutex_entrenadores_ready);
	list_add(entrenadores_ready, entrenador_listo);
	pthread_mutex_unlock(&mutex_entrenadores_ready);

	//Aviso al hilo de planificacion que hay un entrenador en ready
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
	list_add(entrenador_listo->acciones, accion_entrenador);
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
	return dictionary_has_key(objetivo_global,nombre_pokemon) && necesito_mas_de_ese_pokemon(nombre_pokemon);
}

//Si encuentra en el objetivo global, una cantidad mayor a 0 de ese pokemon, entonces lo necesitamos. Al devolver un numero mayor a 0 -> True
int necesito_mas_de_ese_pokemon(char* nombre_pokemon){
	return ((int) dictionary_get(objetivo_global,nombre_pokemon)) > 0;
}


//PARTE DE DETECCION DEADLOCK (FALTA TERMINAR)
void planear_intercambio(entrenador* entrenador1){
	log_info(nuestro_log,"Entre a planear el intercambio");
	intercambio* un_intercambio = malloc(sizeof(intercambio));

	cambiar_estado_entrenador(entrenador1, INTERCAMBIO);

	if(se_encontraron_entrenadores_para_intercambio(entrenador1, un_intercambio)){
		log_info(nuestro_log,"Se movera al entrenador %d para hacer intercambio", entrenador1->id);

		list_add(intercambios, un_intercambio);
		//Este cambio se hace simplemente para que si hay muchos entrenadores con estado BLOCK_DEADLOCK, no se los agarre en distintos intercambios
		cambiar_estado_entrenador(un_intercambio->entrenador2, INTERCAMBIO);

		calcular_distancia_en_x_del_entrenador_a_la_posicion(entrenador1, un_intercambio->entrenador2->posicion);
		calcular_distancia_en_y_del_entrenador_a_la_posicion(entrenador1, un_intercambio->entrenador2->posicion);

		agregar_accion(entrenador1, intercambiar, 5);
		cambiar_estado_entrenador(entrenador1, READY);
		agregar_entrenador_a_lista_entrenadores_ready(entrenador1);
	} else {
		log_info(nuestro_log,"No se encontro intercambio beneficioso para entrenador %d", entrenador1->id);
		cambiar_estado_entrenador(entrenador1, BLOCK_DEADLOCK);
		free (un_intercambio);
	}

	log_info(nuestro_log,"Sali de planear el intercambio");

}

int se_encontraron_entrenadores_para_intercambio(entrenador* entrenador1, intercambio* un_intercambio){
	int se_encontro_entrenador = 0;
	entrenador* entrenador_a_intercambiar;

	t_list* entrenadores_en_deadlock = entrenadores_con_block_deadlock();

	for(int i = 0; i < list_size(entrenadores_en_deadlock); i++){

		if(el_otro_entrenador_tiene_el_pokemon_que_necesito(entrenador1, list_get(entrenadores_en_deadlock,i))){

			if(el_otro_entrenador_tiene_el_pokemon_que_necesito(list_get(entrenadores_en_deadlock,i),entrenador1)){
				entrenador_a_intercambiar = list_get(entrenadores_en_deadlock,i);
				un_intercambio->entrenador1 = entrenador1;
				un_intercambio->entrenador2 = entrenador_a_intercambiar;

				t_list* pokemons_que_necesito_para_intercambiar = pokemons_a_intercambiar_ideal(entrenador1,entrenador_a_intercambiar);

				un_intercambio->pokemon1 = list_get(pokemons_que_necesito_para_intercambiar,0);
				un_intercambio->pokemon2 = list_get(pokemons_que_necesito_para_intercambiar,1);

				list_destroy(pokemons_que_necesito_para_intercambiar);
				list_destroy(entrenadores_en_deadlock);

				//CALCULAR POKEMON A INTERCAMBIAR ENTRE AMBAS PARTES

				return 1;
			} else {
				entrenador_a_intercambiar = list_get(entrenadores_en_deadlock,i);
				se_encontro_entrenador = 1;
			}
		}
	}

	if(se_encontro_entrenador == 1){
		un_intercambio->entrenador1 = entrenador1;
		un_intercambio->entrenador2 = entrenador_a_intercambiar;

		//CALCULAR POKEMON A INTERCAMBIAR QUE LE SIRVA AL ENTRENADOR1 SOLAMENTE
		t_list* pokemons_que_necesito_para_intercambiar = pokemons_a_intercambiar(entrenador1,entrenador_a_intercambiar);
		un_intercambio->pokemon1 = list_get(pokemons_que_necesito_para_intercambiar,0);
		//EN POKEMON2 VOY A PONER A POKEMON SOBRANTE DEL ENTRENADOR1
		un_intercambio->pokemon2 = list_get(entrenador1->pokemons_sobrantes,0);

		list_destroy(pokemons_que_necesito_para_intercambiar);
		list_destroy(entrenadores_en_deadlock);
		return 1;
	} else {
		list_destroy(entrenadores_en_deadlock);
		return 0;
	}
}

int el_otro_entrenador_tiene_el_pokemon_que_necesito(entrenador* entrenador1,entrenador* entrenador_a_evaluar){
	for(int i = 0; i< list_size(entrenador1->pokemons_objetivo);i++){
		for(int j=0; j< list_size(entrenador_a_evaluar->pokemons_sobrantes);j++){
			if(string_equals_ignore_case(list_get(entrenador1->pokemons_objetivo,i),list_get(entrenador_a_evaluar->pokemons_sobrantes,j))){
				return 1;
			}
		}
	}

	return 0;
}

//Devuelve un pokemon que el entrenador 1 necesita, sin importar lo que necesite el entrenador 2.
char* pokemon_que_sirven_para_intercambiar(entrenador* entrenador1, entrenador* entrenador2){
	for(int i = 0; i< list_size(entrenador1->pokemons_objetivo);i++){
		for(int j=0; j< list_size(entrenador2->pokemons_sobrantes);j++){
			if(string_equals_ignore_case(list_get(entrenador1->pokemons_objetivo,i),list_get(entrenador2->pokemons_sobrantes,j))){
				return list_get(entrenador1->pokemons_objetivo,i);
			}
		}
	}
}

t_list* pokemons_a_intercambiar(entrenador* entrenador1,entrenador* entrenador2){

	t_list* pokemons_a_intercambiar = list_create();

	//Agrego a la lista de intercambiar al pokemon que el entrenador 1 necesita
	list_add(pokemons_a_intercambiar,pokemon_que_sirven_para_intercambiar(entrenador1,entrenador2));

	//Agrego a la lista de intercambiar a cualquier pokemon que le sobre al entrenador 1.
	list_add(pokemons_a_intercambiar,entrenador1->pokemons_sobrantes);

	return pokemons_a_intercambiar;

}

t_list* pokemons_a_intercambiar_ideal(entrenador* entrenador1,entrenador* entrenador2){

	t_list* pokemons_a_intercambiar = list_create();

	//Agrego a la lista de intercambiar al pokemon que el entrenador 1 necesita
	list_add(pokemons_a_intercambiar,pokemon_que_sirven_para_intercambiar(entrenador1,entrenador2));

	//Agrego a la lista de intercambiar al pokemon que el entrenador 2 necesita
	list_add(pokemons_a_intercambiar,pokemon_que_sirven_para_intercambiar(entrenador2,entrenador1));

	return pokemons_a_intercambiar;

}

int el_entrenador_cumplio_su_objetivo(entrenador* entrenador){
	return list_is_empty(entrenador->pokemons_objetivo);
}

int el_entrenador_esta_block_deadlock(entrenador* entrenador){
	return entrenador->estado == BLOCK_DEADLOCK;
}

int cantidad_del_mismo_pokemon_por_entrenador(t_list* lista_de_pokemons,char* pokemon){
	int cantidad = 0;

	for(int i = 0; i< list_size(lista_de_pokemons); i++){
		if(pokemon == list_get(lista_de_pokemons,i)){
			cantidad++;
		}
	}
	return cantidad;
}

int tiene_mas_cantidad_de_ese_pokemon(t_list* pokemons_adquiridos, t_list* pokemons_objetivo, char* pokemon){
	return cantidad_del_mismo_pokemon_por_entrenador(pokemons_adquiridos, pokemon) > cantidad_del_mismo_pokemon_por_entrenador(pokemons_objetivo, pokemon);
}

t_list* entrenadores_con_block_deadlock(){
	return list_filter(entrenadores,el_entrenador_esta_block_deadlock);
}

int el_entrenador_esta_en_exit(entrenador* entrenador){
	return entrenador->estado == EXIT;
}

int terminaron_todos_los_entrenadores(){
	return list_all_satisfy(entrenadores,el_entrenador_esta_en_exit);
}

int el_entrenador_no_puede_capturar_mas_pokemons(entrenador* entrenador){
	return (list_size(entrenador->pokemons_adquiridos)) >= entrenador->cant_maxima_pokemons;
}

//Realiza un intercambio entre dos entrenadores que estaban bloqueados
void intercambiar(entrenador* entrenador1) {
	log_info(nuestro_log,string_from_format("Empezando la accion de intercambio"));
	char* pokemon_a_eliminar_en_1;
	char* pokemon_a_eliminar_en_2;
	int posicion_pokemon_a_eliminar;


	intercambio* intercambio_a_realizar = buscar_intercambio_correspondiente_al_entrenador(entrenador1);

	posicion_pokemon_a_eliminar = devolver_posicion_en_la_lista_del_pokemon(entrenador1->pokemons_adquiridos,intercambio_a_realizar->pokemon2);
	pokemon_a_eliminar_en_1 = list_remove(entrenador1->pokemons_adquiridos, posicion_pokemon_a_eliminar);
	posicion_pokemon_a_eliminar = devolver_posicion_en_la_lista_del_pokemon(entrenador1->pokemons_sobrantes,intercambio_a_realizar->pokemon2);
	pokemon_a_eliminar_en_1 = list_remove(entrenador1->pokemons_sobrantes, posicion_pokemon_a_eliminar);

	posicion_pokemon_a_eliminar = devolver_posicion_en_la_lista_del_pokemon(intercambio_a_realizar->entrenador2->pokemons_adquiridos, intercambio_a_realizar->pokemon1);
	pokemon_a_eliminar_en_2 = list_remove(intercambio_a_realizar->entrenador2->pokemons_adquiridos,posicion_pokemon_a_eliminar);
	posicion_pokemon_a_eliminar = devolver_posicion_en_la_lista_del_pokemon(intercambio_a_realizar->entrenador2->pokemons_sobrantes,intercambio_a_realizar->pokemon1);
	pokemon_a_eliminar_en_2 = list_remove(intercambio_a_realizar->entrenador2->pokemons_sobrantes, posicion_pokemon_a_eliminar);


	log_info(nuestro_log, string_from_format("Entrenador %d dando %s", entrenador1->id, pokemon_a_eliminar_en_1));
	log_info(nuestro_log, string_from_format("Entrenador %d dando %s", intercambio_a_realizar->entrenador2->id, pokemon_a_eliminar_en_2));

	agregar_pokemon_a_adquirido(entrenador1, pokemon_a_eliminar_en_2);
	agregar_pokemon_a_adquirido(intercambio_a_realizar->entrenador2, pokemon_a_eliminar_en_1);

	accionar_en_funcion_del_estado_del_entrenador(entrenador1);
	accionar_en_funcion_del_estado_del_entrenador(intercambio_a_realizar->entrenador2);

	destruir_intercambio(intercambio_a_realizar);

	log_info(nuestro_log,string_from_format("Terminando la accion de intercambio"));
}

void destruir_intercambio(intercambio* intercambio_realizado) {
	for(int i = 0; i < list_size(intercambios); i++) {
		intercambio* intercambio_aux = list_get(intercambios, i);
		if(intercambio_aux->entrenador1->id == intercambio_realizado->entrenador1->id) {
			intercambio_realizado = list_remove(intercambios, i);
			free(intercambio_realizado);
			break;
		}
	}

}

intercambio* buscar_intercambio_correspondiente_al_entrenador(entrenador* entrenador){
	for(int i = 0; i< list_size(intercambios);i++){
		intercambio* intercambio_a_evaluar = list_get(intercambios,i);
		if(entrenador->id == intercambio_a_evaluar->entrenador1->id){
			return intercambio_a_evaluar;
		}
	}
	return NULL;
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
			liberar_conexion(socket_get);
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
	if(funciona_broker == 0 || socket_catch == -1) {
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
void accionar_en_funcion_del_estado_del_entrenador(entrenador* entrenador){

	if(el_entrenador_cumplio_su_objetivo(entrenador)) {
		log_info(nuestro_log,string_from_format("El entrenador %d cumplio su objetivo y queda en estado EXIT", entrenador->id));
		cambiar_estado_entrenador(entrenador, EXIT);
	} else if (el_entrenador_no_puede_capturar_mas_pokemons(entrenador)) {
		log_info(nuestro_log,string_from_format("El entrenador %d se bloquea quedando en estado DEADLOCK", entrenador->id));
		cambiar_estado_entrenador(entrenador, BLOCK_DEADLOCK);
	} else {
		log_info(nuestro_log,string_from_format("El entrenador %d queda en BLOCK_READY", entrenador->id));
		cambiar_estado_entrenador(entrenador, BLOCK_READY);
	}
}

void manejar_la_captura_del_pokemon(entrenador* entrenador) {
	pokemon* pokemon_en_captura = entrenador->pokemon_en_busqueda;
	log_info(logger, string_from_format("3. Se realiza la captura del pokemon %s, en la posicion %d|%d exitosamente.", pokemon_en_captura->nombre, pokemon_en_captura->posicion->posicion_x, pokemon_en_captura->posicion->posicion_y));
	log_info(nuestro_log, string_from_format("3. Se realiza la captura del pokemon %s, en la posicion %d|%d exitosamente.", pokemon_en_captura->nombre, pokemon_en_captura->posicion->posicion_x, pokemon_en_captura->posicion->posicion_y));
	restar_adquirido_a_objetivo_global(pokemon_en_captura->nombre);
	agregar_pokemon_a_adquirido(entrenador, pokemon_en_captura->nombre);
	destruir_pokemon(pokemon_en_captura);

	accionar_en_funcion_del_estado_del_entrenador(entrenador);
}

void agregar_pokemon_a_adquirido(entrenador* entrenador, char* pokemon_adquirido) {
	list_add(entrenador->pokemons_adquiridos, pokemon_adquirido);
	actualizar_objetivo_y_sobrante_del_entrenador(entrenador, pokemon_adquirido);
}

void destruir_pokemon(pokemon* pokemon) {
	free(pokemon->posicion);
	free(pokemon);
}
