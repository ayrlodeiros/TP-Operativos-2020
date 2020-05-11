#include"utils.h"


//CONEXIONES
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

void conectar_broker(void) {
	char* ip_broker = leer_ip_broker();
	char* puerto_broker = leer_puerto_broker();

	while(1) {
		pthread_mutex_lock(&lock_de_conexion_broker);
		log_info(nuestro_log, string_from_format("Intentando conectar con broker en IP: %s y PUERTO: %s", ip_broker, puerto_broker));
		log_info(logger, "10. Inicio de proceso de intento de comunicación con el Broker.");

		socket_broker = crear_conexion_como_cliente(ip_broker, puerto_broker);

		pthread_mutex_unlock(&lock_de_conexion_broker);

		if(socket_broker == -1) {
			log_info(logger, "11. Intento de conexion con el broker fallida. Se volvera a intentar.");
		} else {
			log_info(logger, "11. Intento de conexion con el broker exitosa.");
			pthread_mutex_lock(&lock_de_conexion_broker);
		}

		sleep(leer_tiempo_reconexion());
	}
}

int funciona_la_conexion_con_broker() {
	if(socket_broker == -1) {
		pthread_mutex_unlock(&lock_de_conexion_broker);
		return 0;
	} else {
		return 1;
	}
}

//VER DE INICIAR TODO LO DE EL REINTENGO EN ESTE METODO, ADEMAS DE INICIALIZAR EL PTHREAD ACA
void intentar_conectar_al_broker() {

	pthread_t* conexion_broker;

	pthread_create(&conexion_broker,NULL,conectar_broker, NULL);
	//NO LO ESPERO
	pthread_detach(conexion_broker);
	//LO ESPERO
	//pthread_join(conexion_broker, NULL);

	//ESTO ES SOLO PARA PROBAR
	while(1) {
		log_info(nuestro_log, string_from_format("VALOR DEL SOCKET: %d", socket_broker));
		if(funciona_la_conexion_con_broker()){
			log_info(nuestro_log, "FUNCIONA");
		} else {
			log_info(nuestro_log, "NO FUNCIONA");
		}
		sleep(1);
	}
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
	pokemon* nuevo_pokemon  = malloc(sizeof(pokemon));;
	nuevo_pokemon->nombre = nombre;
	nuevo_pokemon->posicion = armar_posicion(string_from_format("%d|%d", posicion_x, posicion_y));

	list_add(pokemons_sueltos, nuevo_pokemon);
	buscar_entrenador_a_planificar(nuevo_pokemon);
}


void buscar_entrenador_a_planificar(pokemon* pokemon_objetivo){
	//Seteo la variable global del utils para poder manejarla en los distintos metodos
	pokemon_para_planificar = pokemon_objetivo;

	//Filtro entrenadores en estado NEW o BLOCK_READY, luego ordeno la lista para obtener al primero mas cercano y despues lo agrego a la lista de entredores ready
	agregar_entrenador_a_entrenadores_ready(list_get(list_sorted(list_filter(entrenadores, el_entrenador_se_puede_planificar), el_entrenador1_esta_mas_cerca), 0));
}

void agregar_entrenador_a_entrenadores_ready(entrenador* entrenador_listo) {
	cambiar_estado_entrenador(entrenador_listo, READY);
	list_add(entrenadores_ready, entrenador_listo);
	//AGREGAR ACCIONES AL ENTRENADOR PARA QUE SE MUEVA A LA POSICION
	printf("AGREGAR ACCIONES AL ENTRENADOR PARA QUE SE MUEVA A LA POSICION");
	pthread_mutex_unlock(&lock_de_planificacion);
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





int el_pokemon_es_requerido(char* nombre_pokemon){
	return dictionary_has_key(objetivo_global,nombre_pokemon)
			&& necesito_mas_de_ese_pokemon(nombre_pokemon);
}

//Si encuentra en el objetivo global, una cantidad mayor a 0 de ese pokemon, entonces lo necesitamos. Al devolver un numero mayor a 0 -> True
int necesito_mas_de_ese_pokemon(char* nombre_pokemon){
	return ((int) dictionary_get(objetivo_global,nombre_pokemon)) > 0;
}


