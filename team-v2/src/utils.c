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

//FIN DE CONEXIONES
//
//
//
void appeared_pokemon(char* nombre_pokemon,int posicion_x,int posicion_y){
	if(el_pokemon_es_requerido(nombre_pokemon)){
		armar_pokemon(nombre_pokemon,posicion_x,posicion_y);
	}
}

t_list* get_pokemon(char* nombre_pokemon){

}

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado){
	entrenador->estado = un_estado;
}

int el_entrenador_se_puede_planificar(entrenador* un_entrenador){
	return un_entrenador->estado == READY;
}

entrenador* entrenador_a_ejecutar(){
	//Consigo los entrenador que estan en ready
	t_list* entrenadores_a_ejecutar = list_filter(entrenadores, el_entrenador_se_puede_planificar);

	//Ordeno los entrenadores en funcion de que tan cerca estan del pokemon
	list_sort(entrenadores_a_ejecutar, el_entrenador1_esta_mas_cerca);

	for(int i = 0; i < list_size(entrenadores_a_ejecutar); i++){
			entrenador* entrenador = list_get(entrenadores_a_ejecutar, i);
			printf("\nPOSICION ENTRENADOR %d: X->%d e Y->%d", i, entrenador->posicion->posicion_x, entrenador->posicion->posicion_y);
			printf("\nCANTIDAD MAXIMA POKEMONS ENTRENADOR %d: %d", i, entrenador->cant_maxima_pokemons);
			for(int j = 0; j<list_size(entrenador->pokemons_adquiridos); j++){
				printf("\nPOKEMONS ENTRENADOR %d: %s", i, list_get(entrenador->pokemons_adquiridos, j));
			}
		}

}


int distancia_del_entrenador_al_pokemon(entrenador* entrenador, pokemon* pokemon) {
	return (int) (fabs(entrenador->posicion->posicion_x - pokemon->posicion->posicion_x)) + (int) (fabs(entrenador->posicion->posicion_y - pokemon->posicion->posicion_y));
}

int el_entrenador1_esta_mas_cerca(entrenador* entrenador1, entrenador* entrenador2) {
	int distancia_al_pokemon_entrenador1 = distancia_del_entrenador_al_pokemon(entrenador1,queue_peek(pokemons_sueltos));
	int distancia_al_pokemon_entrenador2 = distancia_del_entrenador_al_pokemon(entrenador2,queue_peek(pokemons_sueltos));

	if(distancia_al_pokemon_entrenador1 <= distancia_al_pokemon_entrenador2){
		printf("\n El resultado de la funcion es: 1");
		return 1;
	}
	else {
		printf("\n El resultado de la funcion es: 0");
		return 0;
	}

}

void restar_cpu_disponible(entrenador* entrenador, int cantidad) {
	entrenador->cpu_disponible -= cantidad;
}

void sumar_cpu_usado(entrenador* entrenador, int cantidad) {
	entrenador->cpu_usado += cantidad;
}



int el_pokemon_es_requerido(char* nombre_pokemon){
	return dictionary_has_key(objetivo_global,nombre_pokemon)
			&& necesito_mas_de_ese_pokemon(nombre_pokemon);
}

//Si encuentra en el objetivo global, una cantidad mayor a 0 de ese pokemon, entonces lo necesitamos. Al devolver un numero mayor a 0 -> True
int necesito_mas_de_ese_pokemon(char* nombre_pokemon){
	return ((int) dictionary_get(objetivo_global,nombre_pokemon)) > 0;
}


