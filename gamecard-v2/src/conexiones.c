#include "conexiones.h"

void levantar_conexiones() {
	pthread_mutex_init(&lock_reintento_broker, NULL);
	pthread_mutex_init(&mutex_funciona_broker, NULL);

	pthread_mutex_lock(&lock_reintento_broker);

	conexion_new = -1;
	conexion_catch = -1;
	conexion_get = -1;
	funciona_broker=0;

	pthread_t* hilo_gameboy;
	pthread_create(&hilo_gameboy,NULL, atender_conexion_gameboy, NULL);
	pthread_detach(hilo_gameboy);

	pthread_t* hilo_broker;
	pthread_create(&hilo_broker,NULL, levantar_conexiones_al_broker, NULL);
	pthread_detach(hilo_broker);
}

t_paquete* crear_paquete(codigo_operacion cod_op, codigo_accion cod_acc, int id_mensaje, t_buffer* buffer) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->numero_de_modulo = leer_id_modulo();
	paquete->codigo_de_operacion = cod_op;
	paquete->codigo_de_accion = cod_acc;
	paquete->id_mensaje = id_mensaje;
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
	int id_modulo = leer_id_modulo();

	memcpy(a_enviar + offset, &id_modulo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->codigo_de_operacion), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->codigo_de_accion), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->id_mensaje), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->buffer->tamanio), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,paquete->buffer->stream, paquete->buffer->tamanio);

	return a_enviar;
}

void* serializar_paquete_sin_buffer(t_paquete* paquete, int tamanio_a_enviar){
	void * a_enviar = malloc(tamanio_a_enviar);
	int offset = 0;
	int id_modulo = leer_id_modulo();

	memcpy(a_enviar + offset, &id_modulo,sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->codigo_de_operacion),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->codigo_de_accion),sizeof(uint32_t));

	return a_enviar;
}

int obtener_tamanio_de_paquete(t_paquete* paquete) {
	return paquete->buffer->tamanio + 5*sizeof(uint32_t);
}

void destruir_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

int crear_conexion_como_cliente(char *ip, int puerto_i) {
	struct addrinfo hints;
	struct addrinfo *server_info;

	char* puerto = string_itoa(puerto_i);

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

	log_info(nuestro_log, "Servidor levantado en IP: %s y PUERTO: %s", ip, puerto);

	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);

	return socket_servidor;
}

//PARA GAMEBOY
void atender_conexion_gameboy() {
	int servidor_para_gameboy = levantar_servidor(leer_ip_gamecard(), string_itoa(leer_puerto_gamecard()));

	while (1) {
		struct sockaddr_in dir_cliente;

		int tam_direccion = sizeof(struct sockaddr_in);

		log_info(nuestro_log, "Listo para recibir conexion de GAMEBOY, ");
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
	if(recv(socket_gameboy, &cod_modulo, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el codigo de modulo");
	}
	codigo_operacion cod_op;
	if(recv(socket_gameboy, &cod_op, sizeof(codigo_operacion), MSG_WAITALL) == -1) {
		hubo_error = 1;;
		log_info(nuestro_log, "No se pudo recibir el codigo de operacion");
	}
	codigo_accion cod_acc;
	if(recv(socket_gameboy, &cod_acc, sizeof(codigo_accion), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el codigo de accion");
	}
	int id_mensaje;
	if(recv(socket_gameboy, &id_mensaje, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el id de mensaje");
	}

	switch(cod_acc) {
		case NEW:
			log_info(nuestro_log, "Se recibio un mensaje NEW del gameboy");
			procesar_mensaje_new(hubo_error, socket_gameboy, id_mensaje);
			break;
		case CATCH:
			log_info(nuestro_log, "Se recibio un mensaje CATCH del gameboy");
			procesar_mensaje_catch(hubo_error, socket_gameboy, id_mensaje);
			break;
		case GET:
			log_info(nuestro_log, "Se recibio un mensaje GET del gameboy");
			procesar_mensaje_get(hubo_error, socket_gameboy, id_mensaje);
			break;
		default:
			log_info(nuestro_log, "Se recibio el codigo de accion: %d, el cual no es valido para el Game Card", cod_acc);
	}
}

void procesar_mensaje_new(int hubo_error, int socket_gameboy, int id_mensaje) {
	int tamanio_stream;
	if(recv(socket_gameboy, &tamanio_stream, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el tamanio del stream");
	}

	int largo_nombre_pokemon;
	if(recv(socket_gameboy, &largo_nombre_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el largo del nombre del pokemon");
	}

	char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
	int posicion_x;
	int posicion_y;
	int cantidad;
	if(recv(socket_gameboy, nombre_pokemon, largo_nombre_pokemon + 1, MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el nombre del pokemon");
	}
	if(recv(socket_gameboy, &posicion_x, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir la posicion en X");
	}
	if(recv(socket_gameboy, &posicion_y, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir la posicion en Y");
	}
	if(recv(socket_gameboy, &cantidad, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir la cantidad");
	}

	if(hubo_error == 0) {
		guardar_informacion(nombre_pokemon, posicion_x, posicion_y, cantidad);
		enviar_mensaje_appeared(id_mensaje, largo_nombre_pokemon, nombre_pokemon, posicion_x, posicion_y);
	} else {
		log_info(nuestro_log, "No se pudo recibir el mensaje NEW del Game Boy");
	}

	free(nombre_pokemon);
}

void procesar_mensaje_catch(int hubo_error, int socket_gameboy, int id_mensaje) {
	int tamanio_stream;
	if(recv(socket_gameboy, &tamanio_stream, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el tamanio del stream");
	}

	int largo_nombre_pokemon;
	if(recv(socket_gameboy, &largo_nombre_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el largo del nombre del pokemon");
	}

	char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
	int posicion_x;
	int posicion_y;
	if(recv(socket_gameboy, nombre_pokemon, largo_nombre_pokemon + 1, MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el nombre del pokemon");
	}
	if(recv(socket_gameboy, &posicion_x, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir la posicion en X");
	}
	if(recv(socket_gameboy, &posicion_y, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir la posicion en Y");
	}

	if(hubo_error == 0) {
		int resultado_del_catch;
		resultado_del_catch = disminuir_cantidad_de_pokemon_en_la_posicion(nombre_pokemon,posicion_x,posicion_y);

		log_info(nuestro_log, "EL RESULTADO DEL CATCH PARA %s ES DE: %d", nombre_pokemon, resultado_del_catch);

		enviar_mensaje_caught(id_mensaje, resultado_del_catch);
	} else {
		log_info(nuestro_log, "No se pudo recibir el mensaje CATCH del Game Boy");
	}
	free(nombre_pokemon);
}

void procesar_mensaje_get(int hubo_error, int socket_gameboy, int id_mensaje) {
	int tamanio_stream;
	if(recv(socket_gameboy, &tamanio_stream, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el tamanio del stream");
	}

	int largo_nombre_pokemon;
	if(recv(socket_gameboy, &largo_nombre_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el largo del nombre del pokemon");
	}

	char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
	if(recv(socket_gameboy, nombre_pokemon, largo_nombre_pokemon + 1, MSG_WAITALL) == -1) {
		hubo_error = 1;
		log_info(nuestro_log, "No se pudo recibir el nombre del pokemon");
	}

	if(hubo_error == 0) {
		t_list* datos = armar_mensaje_get(nombre_pokemon);
		int cantidad_de_posiciones = list_size(datos);
		t_list* posiciones = armar_lista_de_posiciones_del_pokemon(datos);

		log_info(nuestro_log, "HAY %d POSICIONES DISTINTAS EN LA QUE APARECE %s", cantidad_de_posiciones, nombre_pokemon);

		enviar_mensaje_localized(id_mensaje, largo_nombre_pokemon, nombre_pokemon, cantidad_de_posiciones, posiciones);

		list_destroy_and_destroy_elements(datos,free);
		list_destroy_and_destroy_elements(posiciones, free);
	} else {
		log_info(nuestro_log, "No se pudo recibir el mensaje GET del Game Boy");
	}
	free(nombre_pokemon);
}

t_list* armar_lista_de_posiciones_del_pokemon(t_list* datos) {
	t_list* posiciones = list_create();
	for(int i = 0; i<list_size(datos); i++) {
		char* dato = list_get(datos, i);
		posicion* pos = malloc(sizeof(posicion));
		pos->posicion_x = obtener_posicion_x_del_dato(dato);
		pos->posicion_y = obtener_posicion_y_del_dato(dato);
		list_add(posiciones, pos);
	}
	return posiciones;
}

//PARA BROKER
int intentar_conectar_al_broker() {
	char* ip_broker = leer_ip_broker();
	int puerto_broker = leer_puerto_broker();
	int conexion = -1;

	while(conexion == -1) {
		log_info(nuestro_log, string_from_format("Intentando conectar con broker en IP: %s y PUERTO: %d", ip_broker, puerto_broker));
		log_info(logger, "Inicio de proceso de intento de comunicación con el Broker.");

		conexion = crear_conexion_como_cliente(ip_broker, puerto_broker);

		if(conexion == -1) {
			log_info(logger, "Intento de conexion con el broker fallida. Se volvera a intentar.");
			sleep(leer_tiempo_de_reintento_conexion());
		} else {
			log_info(logger, "Intento de conexion con el broker exitosa.");
		}
	}
	return conexion;
}

void levantar_conexiones_al_broker() {
	pthread_t* hilo_new;
	pthread_t* hilo_catch;
	pthread_t* hilo_get;
	while(1) {
		if(funciona_broker == 1) {
			log_info(nuestro_log, "Funciona broker, activando las tres colas...");

			pthread_create(&hilo_new,NULL, esperar_mensaje_new, NULL);
			pthread_detach(hilo_new);
			pthread_create(&hilo_catch,NULL, esperar_mensaje_catch, NULL);
			pthread_detach(hilo_catch);
			pthread_create(&hilo_get,NULL, esperar_mensaje_get, NULL);
			pthread_detach(hilo_get);

			log_info(nuestro_log, "Bloqueo de conexiones al broker");

			pthread_mutex_lock(&lock_reintento_broker);

			log_info(nuestro_log, "Desbloqueo de conexiones al broker");
		} else {

			log_info(nuestro_log, "El broker no esta conectado...");

			conexion_new = intentar_conectar_al_broker();

			conexion_catch = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());

			conexion_get = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());

			if(conexion_new == -1 || conexion_catch == -1 || conexion_get == -1) {
				cambiar_valor_de_funciona_broker(0);
			} else {
				cambiar_valor_de_funciona_broker(1);
			}
		}

	}
}

void esperar_mensaje_new() {
	suscribirse_a_cola(conexion_new, NEW);
	while(funciona_broker == 1) {
		log_info(nuestro_log, "Estoy esperando mensaje NEW");
		mensaje_broker* msj_broker = recibir_msj_broker(conexion_new);
		if(msj_broker == NULL) {
			log_info(nuestro_log, "Se perdio la conexion con el broker");
		} else {
			pthread_t* hilo_mensaje_new;
			pthread_create(&hilo_mensaje_new, NULL, trabajar_mensaje_new, msj_broker);
			pthread_detach(hilo_mensaje_new);
		}
	}
}

void trabajar_mensaje_new(mensaje_broker* msj_broker) {
	int id_mensaje = msj_broker->id;
	void* payload = msj_broker->payload;
	int offset = 0;

	int largo_nombre_pokemon;
	memcpy(&largo_nombre_pokemon, payload+offset, sizeof(uint32_t));
	offset+=sizeof(uint32_t);

	char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
	memcpy(nombre_pokemon, payload+offset, largo_nombre_pokemon+1);
	offset+=(largo_nombre_pokemon+1);

	int posicion_x;
	memcpy(&posicion_x, payload+offset, sizeof(uint32_t));
	offset+=sizeof(uint32_t);

	int posicion_y;
	memcpy(&posicion_y, payload+offset, sizeof(uint32_t));
	offset+=sizeof(uint32_t);

	int cantidad;
	memcpy(&cantidad, payload+offset, sizeof(uint32_t));
	offset+=sizeof(uint32_t);

	guardar_informacion(nombre_pokemon, posicion_x, posicion_y, cantidad);

	enviar_mensaje_appeared(id_mensaje, largo_nombre_pokemon, nombre_pokemon, posicion_x, posicion_y);

	free(nombre_pokemon);
	free(msj_broker->payload);
	free(msj_broker);
}

void enviar_mensaje_appeared(int id_mensaje, int largo_nombre_pokemon, char* nombre_pokemon, int posicion_x, int posicion_y) {
	int socket_appeared = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(socket_appeared == -1) {
		//ACCION POR DEFAULT
		log_info(logger, "No se pudo realizar la conexión con el BROKER para el envio del mensaje APPEARED de %s", nombre_pokemon);
		log_info(nuestro_log, "No se pudo realizar la conexión con el BROKER para el envio del mensaje APPEARED de %s", nombre_pokemon);
	} else {
		//ACCION CON EL BROKER

		//CREO EL BUFFER CON SU TAMANIO Y STREAM
		t_buffer* buffer = malloc(sizeof(t_buffer));

		buffer->tamanio = largo_nombre_pokemon + 1 + (3*sizeof(uint32_t));

		void* stream = malloc(buffer->tamanio);
		int offset = 0;
		memcpy(stream + offset,&largo_nombre_pokemon, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset,nombre_pokemon, (largo_nombre_pokemon+1));
		offset += (largo_nombre_pokemon+1);
		memcpy(stream + offset,&posicion_x, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset,&posicion_y, sizeof(uint32_t));

		buffer->stream = stream;

		//CREO EL PAQUETE CON EL CONTENIDO DE LO QUE VOY A ENVIAR
		t_paquete* paquete = crear_paquete(MENSAJE, APPEARED, id_mensaje, buffer);

		//SERIALIZO EL PAQUETE, LO MANDO y ESPERO LA RESPUESTA DEL BROKER
		int bytes = obtener_tamanio_de_paquete(paquete);
		void* a_enviar = serializar_paquete(paquete, bytes);

		if(send(socket_appeared, a_enviar, bytes ,0) > 0){
			log_info(nuestro_log, "Se realizo el envio del APPEARED de %s correctamente", nombre_pokemon);

			int id_respuesta;
			if(recv(socket_appeared, &id_respuesta, sizeof(uint32_t), 0) > 0) {
				log_info(nuestro_log, "Se recibio el id de respuesta: %d del mensaje APPEARED de %s correctamente", id_respuesta, nombre_pokemon);
			} else {
				log_info(logger, "No se pudo recibir el id de respuesta del mensaje APPEARED de %s", nombre_pokemon);
				log_info(nuestro_log, "No se pudo recibir el id de respuesta del mensaje APPEARED de %s", nombre_pokemon);
			}

		} else{
			log_info(logger, "No se pudo realizar la conexión con el BROKER para el envio del mensaje APPEARED de %s", nombre_pokemon);
			log_info(nuestro_log, "No se pudo realizar la conexión con el BROKER para el envio del mensaje APPEARED de %s", nombre_pokemon);
		}

		free(a_enviar);
		destruir_paquete(paquete);
		liberar_conexion(socket_appeared);
	}
}

void esperar_mensaje_catch() {
	suscribirse_a_cola(conexion_catch, CATCH);
	while(funciona_broker == 1) {
		log_info(nuestro_log, "Estoy esperando mensaje CATCH");
		mensaje_broker* msj_broker = recibir_msj_broker(conexion_catch);
		if(msj_broker == NULL) {
			log_info(nuestro_log, "Se perdio la conexion con el broker");
		} else {
			pthread_t* hilo_mensaje_catch;
			pthread_create(&hilo_mensaje_catch, NULL, trabajar_mensaje_catch, msj_broker);
			pthread_detach(hilo_mensaje_catch);
		}
	}
}

void trabajar_mensaje_catch(mensaje_broker* msj_broker) {
	int id_mensaje = msj_broker->id;
	void* payload = msj_broker->payload;
	int offset = 0;

	log_info(nuestro_log, "El id del mensaje CATCH es %d", id_mensaje);
	int largo_nombre_pokemon;
	memcpy(&largo_nombre_pokemon, payload+offset, sizeof(uint32_t));
	offset+=sizeof(uint32_t);

	char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
	memcpy(nombre_pokemon, payload+offset, largo_nombre_pokemon+1);
	offset+=(largo_nombre_pokemon+1);
	log_info(nuestro_log, "El nombre del pokemon es %s", nombre_pokemon);


	int posicion_x;
	memcpy(&posicion_x, payload+offset, sizeof(uint32_t));
	offset+=sizeof(uint32_t);

	int posicion_y;
	memcpy(&posicion_y, payload+offset, sizeof(uint32_t));
	offset+=sizeof(uint32_t);

	int resultado_del_catch;
	resultado_del_catch = disminuir_cantidad_de_pokemon_en_la_posicion(nombre_pokemon,posicion_x,posicion_y);

	log_info(nuestro_log, "EL RESULTADO DEL CATCH PARA %s ES DE: %d", nombre_pokemon, resultado_del_catch);

	enviar_mensaje_caught(id_mensaje, resultado_del_catch);

	free(nombre_pokemon);
	free(msj_broker->payload);
	free(msj_broker);
}

void enviar_mensaje_caught(int id_mensaje, int resultado) {
	int socket_caught = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(socket_caught == -1) {
		//ACCION POR DEFAULT
		log_info(logger, "No se pudo realizar la conexión con el BROKER para el envio del mensaje CAUGHT de ID: %d", id_mensaje);
		log_info(nuestro_log, "No se pudo realizar la conexión con el BROKER para el envio del mensaje APPEARED de ID: %d", id_mensaje);
	} else {
		//ACCION CON EL BROKER

		//CREO EL BUFFER CON SU TAMANIO Y STREAM
		t_buffer* buffer = malloc(sizeof(t_buffer));

		buffer->tamanio = sizeof(uint32_t);

		void* stream = malloc(buffer->tamanio);
		int offset = 0;
		memcpy(stream + offset,&resultado, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		buffer->stream = stream;

		//CREO EL PAQUETE CON EL CONTENIDO DE LO QUE VOY A ENVIAR
		t_paquete* paquete = crear_paquete(MENSAJE, CAUGHT, id_mensaje, buffer);

		//SERIALIZO EL PAQUETE, LO MANDO y ESPERO LA RESPUESTA DEL BROKER
		int bytes = obtener_tamanio_de_paquete(paquete);
		void* a_enviar = serializar_paquete(paquete, bytes);

		if(send(socket_caught, a_enviar, bytes ,0) > 0){
			log_info(nuestro_log, "Se realizo el envio del CAUGHT, con ID: %d, correctamente", id_mensaje);

			int id_respuesta;
			if(recv(socket_caught, &id_respuesta, sizeof(uint32_t), 0) > 0) {
				log_info(nuestro_log, "Se recibio el id de respuesta: %d del mensaje CAUGHT, con ID: %d, correctamente", id_respuesta, id_mensaje);
			} else {
				log_info(logger, "No se pudo recibir el id de respuesta del mensaje CAUGHT con ID: %d", id_mensaje);
				log_info(nuestro_log, "No se pudo recibir el id de respuesta del mensaje CAUGHT con ID: %d", id_mensaje);
			}
		} else {
			log_info(logger, "No se pudo realizar la conexión con el BROKER para el envio del mensaje CAUGHT con ID: %d", id_mensaje);
			log_info(nuestro_log, "No se pudo realizar la conexión con el BROKER para el envio del mensaje CAUGHT con ID: %d", id_mensaje);
		}

		free(a_enviar);
		destruir_paquete(paquete);
		liberar_conexion(socket_caught);
	}
}

void esperar_mensaje_get() {
	suscribirse_a_cola(conexion_get, GET);

	while(funciona_broker == 1) {
		log_info(nuestro_log, "Estoy esperando mensaje GET");
		mensaje_broker* msj_broker = recibir_msj_broker(conexion_get);
		if(msj_broker == NULL) {
			log_info(nuestro_log, "Se perdio la conexion con el broker");
		} else {
			pthread_t* hilo_mensaje_get;
			pthread_create(&hilo_mensaje_get, NULL, trabajar_mensaje_get, msj_broker);
			pthread_detach(hilo_mensaje_get);
		}
	}
}

void trabajar_mensaje_get(mensaje_broker* msj_broker) {
	int id_mensaje = msj_broker->id;
	void* payload = msj_broker->payload;
	int offset = 0;

	int largo_nombre_pokemon;
	memcpy(&largo_nombre_pokemon, payload+offset, sizeof(uint32_t));
	offset+=sizeof(uint32_t);

	char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
	memcpy(nombre_pokemon, payload+offset, largo_nombre_pokemon+1);
	offset+=(largo_nombre_pokemon+1);

	t_list* datos = armar_mensaje_get(nombre_pokemon);
	int cantidad_de_posiciones = list_size(datos);
	t_list* posiciones = armar_lista_de_posiciones_del_pokemon(datos);

	log_info(nuestro_log, "HAY %d POSICIONES DISTINTAS EN LA QUE APARECE %s", cantidad_de_posiciones, nombre_pokemon);

	enviar_mensaje_localized(id_mensaje, largo_nombre_pokemon, nombre_pokemon, cantidad_de_posiciones, posiciones);

	list_destroy_and_destroy_elements(datos,free);
	list_destroy_and_destroy_elements(posiciones, free);
	free(nombre_pokemon);
	free(msj_broker->payload);
	free(msj_broker);
}

void enviar_mensaje_localized(int id_mensaje, int largo_nombre_pokemon, char* nombre_pokemon, int cantidad_de_posiciones, t_list* posiciones) {
	int socket_localized = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(socket_localized == -1) {
		//ACCION POR DEFAULT
		log_info(logger, "No se pudo realizar la conexión con el BROKER para el envio del mensaje LOCALIZED de %s", nombre_pokemon);
		log_info(nuestro_log, "No se pudo realizar la conexión con el BROKER para el envio del mensaje LOCALIZED de %s", nombre_pokemon);
	} else {
		//ACCION CON EL BROKER

		//CREO EL BUFFER CON SU TAMANIO Y STREAM
		t_buffer* buffer = malloc(sizeof(t_buffer));

		buffer->tamanio = largo_nombre_pokemon + 1 + (2*sizeof(uint32_t)) + (cantidad_de_posiciones*2*sizeof(uint32_t));

		void* stream = malloc(buffer->tamanio);
		int offset = 0;
		memcpy(stream + offset,&largo_nombre_pokemon, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset,nombre_pokemon, (largo_nombre_pokemon+1));
		offset += (largo_nombre_pokemon+1);
		memcpy(stream + offset,&cantidad_de_posiciones, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		for(int i = 0; i < cantidad_de_posiciones; i++) {
			posicion* pos = list_get(posiciones, i);
			memcpy(stream + offset,&(pos->posicion_x), sizeof(uint32_t));
			offset += sizeof(uint32_t);
			memcpy(stream + offset,&(pos->posicion_x), sizeof(uint32_t));
			offset += sizeof(uint32_t);
		}

		buffer->stream = stream;

		//CREO EL PAQUETE CON EL CONTENIDO DE LO QUE VOY A ENVIAR
		t_paquete* paquete = crear_paquete(MENSAJE, LOCALIZED, id_mensaje, buffer);

		//SERIALIZO EL PAQUETE, LO MANDO y ESPERO LA RESPUESTA DEL BROKER
		int bytes = obtener_tamanio_de_paquete(paquete);
		void* a_enviar = serializar_paquete(paquete, bytes);

		if(send(socket_localized, a_enviar, bytes ,0) > 0){
			log_info(nuestro_log, "Se realizo el envio del LOCALIZED de %s correctamente", nombre_pokemon);

			int id_respuesta;
			if(recv(socket_localized, &id_respuesta, sizeof(uint32_t), 0) > 0) {
				log_info(nuestro_log, "Se recibio el id de respuesta: %d del mensaje LOCALIZED de %s correctamente", id_respuesta, nombre_pokemon);
			} else {
				log_info(logger, "No se pudo recibir el id de respuesta del mensaje LOCALIZED de %s", nombre_pokemon);
				log_info(nuestro_log, "No se pudo recibir el id de respuesta del mensaje LOCALIZED de %s", nombre_pokemon);
			}

		} else {
			log_info(logger, "No se pudo realizar la conexión con el BROKER para el envio del mensaje LOCALIZED de %s", nombre_pokemon);
			log_info(nuestro_log, "No se pudo realizar la conexión con el BROKER para el envio del mensaje LOCALIZED de %s", nombre_pokemon);
		}

		free(a_enviar);
		destruir_paquete(paquete);
		liberar_conexion(socket_localized);
	}
}

mensaje_broker* recibir_msj_broker(int conexion_broker) {
	int hubo_error = 0;
	int mallockeo_payload = 0;
	int id;
	int id_cor;
	int tamanio;
	void* payload;
	if(recv(conexion_broker, &id, sizeof(uint32_t), 0) == -1) {
		hubo_error = 1;
	} else {
		if(recv(conexion_broker, &id_cor, sizeof(uint32_t), 0) == -1) {
			hubo_error = 1;
		} else {
			if(recv(conexion_broker, &tamanio, sizeof(uint32_t), 0) == -1) {
				hubo_error = 1;
			} else {
				if(tamanio>0 && tamanio<100000) {
					payload = malloc(tamanio);
					if(recv(conexion_broker, payload, tamanio, 0) == -1) {
						hubo_error = 1;
					}
					mallockeo_payload = 1;
				}
			}
		}
	}

	if(hubo_error || mallockeo_payload == 0) {
		log_info(nuestro_log, "No se pudo recibir el mensaje del broker, se perdio la conexion");
		if(mallockeo_payload) {
			free(payload);
		}

		cambiar_valor_de_funciona_broker(0);
		desbloquear_lock_reintento();

		return NULL;
	} else {
		log_info(nuestro_log, "Se recibio el mensaje del broker correctamente");
		mensaje_broker* msj_broker = malloc(sizeof(mensaje_broker));
		msj_broker->id = id;
		msj_broker->id_correlativo = id_cor;
		msj_broker->tamanio = tamanio;
		msj_broker->payload = payload;

		mandar_ack(conexion_broker, 1);

		return msj_broker;
	}
}

void mandar_ack(int conexion, int resultado) {
	send(conexion, &resultado, sizeof(int), 0);
}

void suscribirse_a_cola(int conexion_broker, codigo_accion cola_a_suscribir) {
	t_paquete* paquete = crear_paquete_sin_buffer(SUSCRIPCION, cola_a_suscribir);
	int tamanio_paquete = 3*sizeof(uint32_t);
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
