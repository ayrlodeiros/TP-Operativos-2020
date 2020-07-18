#include"utils.h"


//CONEXIONES
<<<<<<< HEAD
=======
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

	//Creo el id con valor -1 porque no tiene id correlativo
	int id_correlativo = -1;

	int offset = 0;
	memcpy(a_enviar + offset, &(paquete->numero_de_modulo),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->codigo_de_operacion),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->codigo_de_accion),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&id_correlativo,sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->buffer->tamanio),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,paquete->buffer->stream, paquete->buffer->tamanio);

	return a_enviar;
}

void* serializar_paquete_sin_buffer(t_paquete* paquete, int tamanio_a_enviar){
	void * a_enviar = malloc(tamanio_a_enviar);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->numero_de_modulo),sizeof(uint32_t));
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

>>>>>>> develop
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
<<<<<<< HEAD
		log_error(nuestro_log, string_from_format("Falla al conectarse en IP: %s y PUERTO: %s", ip, puerto));
		freeaddrinfo(server_info);
		return -1;
	} else {
		log_info(nuestro_log, string_from_format("Conexion exitosa en IP: %s y PUERTO: %s", ip, puerto));
=======
		//log_error(nuestro_log, "Falla al conectarse en IP: %s y PUERTO: %s", ip, puerto);
		freeaddrinfo(server_info);
		return -1;
	} else {
		log_info(nuestro_log, "Conexion exitosa en IP: %s y PUERTO: %s", ip, puerto);
>>>>>>> develop
		freeaddrinfo(server_info);
		return socket_cliente;
	}
}

<<<<<<< HEAD
void liberar_conexion(int socket) {
	close(socket);
=======
void liberar_conexion(int conexion) {
	close(conexion);
>>>>>>> develop
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

<<<<<<< HEAD
	log_info(nuestro_log, string_from_format("Servidor levantado en IP: %s y PUERTO: %s", ip, puerto));
=======
	log_info(nuestro_log, "Servidor levantado en IP: %s y PUERTO: %s", ip, puerto);
>>>>>>> develop

	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);

	return socket_servidor;
}

<<<<<<< HEAD
void intentar_conectar_al_broker(int conexion) {
=======
	//PARA GAMEBOY
void atender_conexion_gameboy() {
	int servidor_para_gameboy = levantar_servidor(leer_ip_team(), leer_puerto_team());
	if(servidor_para_gameboy < 0) {
		log_error(nuestro_log, "No se pudo levantar servidor para atender al GAMEBOY en IP: %s y PUERTO: %s", leer_ip_team(), leer_puerto_team());
	} else {
		while (1) {
			struct sockaddr_in dir_cliente;

			int tam_direccion = sizeof(struct sockaddr_in);

			log_info(nuestro_log, "Listo para recibir conexion de GAMEBOY, en servidor: %d", servidor_para_gameboy);
			int socket_cliente = accept(servidor_para_gameboy, (void*) &dir_cliente, &tam_direccion);
			if(socket_cliente < 0) {
				log_info(nuestro_log, "Hubo un error al intentar recibir una conexion en el servidor para el GAMEBOY");
				//TODO VER SI DEJAR ESTO O NO
				log_info(nuestro_log, "Se esperara 10 segundos para poder volver a recibir otra conexion en el servidor para el GAMEBOY");
				sleep(10);
			} else {
				log_info(nuestro_log, "CONEXION REALIZADA CORRECTAMENTE EN SERVIDOR PARA EL GAMEBOY");

				pthread_t* hilo_conexion_gameboy;
				pthread_create(&hilo_conexion_gameboy,NULL, recibir_mensaje_de_gameboy, socket_cliente);
				pthread_detach(hilo_conexion_gameboy);
			}
		}
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
		log_info(nuestro_log, "No se pudo recibir el mensaje del Game Boy");
	}
}
	//FIN DE PARA GAMEBOY
	//
	//

	//PARA BROKER
int intentar_conectar_al_broker() {
>>>>>>> develop
	char* ip_broker = leer_ip_broker();
	char* puerto_broker = leer_puerto_broker();
	int conexion = -1;

	while(conexion == -1) {
		log_info(nuestro_log, "Intentando conectar con broker en IP: %s y PUERTO: %s", ip_broker, puerto_broker);
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

<<<<<<< HEAD
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

/*int funciona_la_conexion_con_broker() {
	if(socket_broker == -1) {
		return 0;
	} else {
		return 1;
	}
}*/

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
=======
void levantar_conexiones_al_broker() {
	pthread_t* hilo_appeared;
	pthread_t* hilo_localized;
	pthread_t* hilo_caught;
	while(1) {
		if(funciona_broker == 1) {
			log_info(nuestro_log, "Funciona broker, activando las tres colas...");

			pthread_create(&hilo_caught,NULL, esperar_mensaje_caught, NULL);
			pthread_detach(hilo_caught);
			pthread_create(&hilo_appeared,NULL, esperar_mensaje_appeared, NULL);
			pthread_detach(hilo_appeared);
			pthread_create(&hilo_localized,NULL, esperar_mensaje_localized, NULL);
			pthread_detach(hilo_localized);

			log_info(nuestro_log, "Bloqueo de conexiones al broker");

			pthread_mutex_lock(&lock_reintento_broker);

			log_info(nuestro_log, "Desbloqueo de conexiones al broker");
		} else {

			log_info(nuestro_log, "El broker no esta conectado...");

			conexion_appeared = intentar_conectar_al_broker();
			log_info(nuestro_log, "La conexion del appeared es %d", conexion_appeared);

			conexion_localized = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
			log_info(nuestro_log, "La conexion del localized es %d", conexion_localized);

			conexion_caught = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
			log_info(nuestro_log, "La conexion del caught es %d", conexion_caught);

			if(conexion_appeared == -1 || conexion_localized == -1 || conexion_caught == -1) {
				cambiar_valor_de_funciona_broker(0);
			} else {
				cambiar_valor_de_funciona_broker(1);
			}
		}

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

	if(hubo_error || mallockeo_payload==0) {
		log_info(nuestro_log, "No se pudo recibir el mensaje del broker, se perdio la conexion");
		if(mallockeo_payload) {
			free(payload);
		}

		cambiar_valor_de_funciona_broker(0);
		desbloquear_lock_reintento();

		return NULL;
	} else {
		log_info(nuestro_log, "Se recibio un mensaje del broker en la conexion %d", conexion_broker);
		mensaje_broker* msj_broker = malloc(sizeof(mensaje_broker));
		msj_broker->id = id;
		log_info(nuestro_log, "El ID del mensaje es: %d", id);
		msj_broker->id_correlativo = id_cor;
		log_info(nuestro_log, "El ID CORRELATIVO del mensaje es: %d", id_cor);
		msj_broker->tamanio = tamanio;
		log_info(nuestro_log, "El TAMANIO del mensaje es: %d", tamanio);
		msj_broker->payload = payload;

		mandar_ack(conexion_broker, 1);

		return msj_broker;
	}
}

void mandar_ack(int conexion, int resultado) {
	send(conexion, &resultado, sizeof(int), 0);
}

void esperar_mensaje_appeared() {
	suscribirse_a_cola(conexion_appeared, APPEARED);
	while(funciona_broker == 1) {
		log_info(nuestro_log, "Estoy esperando mensaje APPEARED");
		mensaje_broker* msj_broker = recibir_msj_broker(conexion_appeared);
		if(msj_broker == NULL) {
			log_info(nuestro_log, "Se perdio la conexion con el broker");
		} else {

			// Deserializacion del payload
			void* payload = msj_broker->payload;
			int offset = 0;

			int largo_nombre_pokemon;
			memcpy(&largo_nombre_pokemon, payload+offset, sizeof(uint32_t));
			offset+=sizeof(uint32_t);

			char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
			memcpy(nombre_pokemon, payload+offset, largo_nombre_pokemon+1);
			offset+=(largo_nombre_pokemon+1);

			actualizar_pokemon_como_recibido(nombre_pokemon);

			int posicion_x;
			memcpy(&posicion_x, payload+offset, sizeof(uint32_t));
			offset+=sizeof(uint32_t);

			int posicion_y;
			memcpy(&posicion_y, payload+offset, sizeof(uint32_t));
			offset+=sizeof(uint32_t);

			// Fin de deserializacion del payload

			manejar_aparicion_de_pokemon(nombre_pokemon, posicion_x, posicion_y);

			free(msj_broker->payload);
			free(msj_broker);
		}
	}
}

void esperar_mensaje_localized() {
	suscribirse_a_cola(conexion_localized, LOCALIZED);
	while(funciona_broker == 1) {
		log_info(nuestro_log, "Estoy esperando mensaje LOCALIZED");
		mensaje_broker* msj_broker = recibir_msj_broker(conexion_localized);
		if(msj_broker == NULL) {
			log_info(nuestro_log, "Se perdio la conexion con el broker");
		} else {
			if(id_esta_en_lista_ids_localized(msj_broker->id_correlativo) == 1) {

				// Deserializacion del payload
				void* payload = msj_broker->payload;
				int offset = 0;

				int largo_nombre_pokemon;
				memcpy(&largo_nombre_pokemon, payload+offset, sizeof(uint32_t));
				offset+=sizeof(uint32_t);

				char* nombre_pokemon = malloc(largo_nombre_pokemon + 1);
				memcpy(nombre_pokemon, payload+offset, largo_nombre_pokemon+1);
				offset+=(largo_nombre_pokemon+1);

				if(pokemon_ya_fue_recibido(nombre_pokemon) == 1) {
					log_info(nuestro_log, "El LOCALIZED de %s no será tenido en cuenta porque, previamente, ya se recibio un mensaje de su aparicion");
					free(nombre_pokemon);
				} else {

					int cantidad;
					memcpy(&cantidad, payload+offset, sizeof(uint32_t));
					offset+=sizeof(uint32_t);

					if(cantidad > 0) {
						actualizar_pokemon_como_recibido(nombre_pokemon);

						t_list* posiciones = list_create();
						for(int i = 0; i<cantidad; i++) {
							posicion* pos = malloc(sizeof(posicion));

							memcpy(&(pos->posicion_x), payload+offset, sizeof(uint32_t));
							offset+=sizeof(uint32_t);

							memcpy(&(pos->posicion_y), payload+offset, sizeof(uint32_t));
							offset+=sizeof(uint32_t);

							list_add(posiciones, pos);

						}
						//Fin de deserializacion del payload


						//TODO REVISAR EL ISSUE PARA SABER SI TOMO TODOS LOS QUE APARECEN EN EL LOCALIZED
						//int cantidad_en_objetivo = (int) dictionary_get(objetivo_global, nombre_pokemon);
						//>>>>>> Por ahora lo dejo para que agarre a todos los pokemons que llegan del localized y los procese <<<<<<

						for(int i=0; i<cantidad; i++) {
							posicion* pos = list_get(posiciones, i);

							manejar_aparicion_de_pokemon(nombre_pokemon, pos->posicion_x, pos->posicion_y);
						}

						list_destroy_and_destroy_elements(posiciones, free);
					} else {
						free(nombre_pokemon);
					}
				}
			}
			free(msj_broker->payload);
			free(msj_broker);
		}
	}
}

int pokemon_ya_fue_recibido(char* pokemon) {
	pthread_mutex_lock(&mutex_pokemons_recibidos);
	for(int i=0; i<list_size(pokemons_recibidos); i++) {
		recepcion_pokemon* pokemon_y_recepcion = list_get(pokemons_recibidos, i);
		if(string_equals_ignore_case(pokemon_y_recepcion->pokemon, pokemon)) {
			pthread_mutex_unlock(&mutex_pokemons_recibidos);
			return pokemon_y_recepcion->fue_recibido_un_msj;
		}
>>>>>>> develop
	}
	pthread_mutex_unlock(&mutex_pokemons_recibidos);
	return 1;
}

<<<<<<< HEAD
void suscribirse_a_cola_caught() {
	int conexion_a_broker = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(conexion_a_broker == -1) {
		intentar_conectar_al_broker(conexion_a_broker);
=======
void actualizar_pokemon_como_recibido(char* pokemon) {
	pthread_mutex_lock(&mutex_pokemons_recibidos);
	for(int i=0; i<list_size(pokemons_recibidos); i++) {
		recepcion_pokemon* pokemon_y_recepcion = list_get(pokemons_recibidos, i);
		if(string_equals_ignore_case(pokemon_y_recepcion->pokemon, pokemon)) {
			pokemon_y_recepcion->fue_recibido_un_msj = 1;
			break;
		}
>>>>>>> develop
	}
	pthread_mutex_unlock(&mutex_pokemons_recibidos);
}

<<<<<<< HEAD
void levantar_conexiones_al_broker() {
	int primera_conexion = crear_conexion_como_cliente(leer_ip_broker(), leer_puerto_broker());
	if(primera_conexion == -1) {
		ejecutar_default = 1;
		pthread_t* reintento_conexion_broker = pthread_create(&reintento_conexion_broker,NULL,intentar_conectar_al_broker, primera_conexion);
		pthread_join(reintento_conexion_broker, NULL);
		ejecutar_default = 0;
	}
	pthread_t* cola_appeared = pthread_create(&cola_appeared,NULL,suscribirse_a_cola_appeared, NULL);
	pthread_t* cola_localized = pthread_create(&cola_localized,NULL,suscribirse_a_cola_localized, NULL);
	pthread_t* cola_caught = pthread_create(&cola_caught,NULL,suscribirse_a_cola_caught, NULL);

	pthread_detach(cola_appeared);
	pthread_detach(cola_localized);
	pthread_detach(cola_caught);
=======
int id_esta_en_lista_ids_localized(int id) {
	pthread_mutex_lock(&mutex_lista_ids_localized);

	for(int i = 0; i<list_size(lista_ids_localized); i++) {
		int id_localized = (int) list_get(lista_ids_localized, i);

		if(id_localized == id) {
			log_info(nuestro_log, "El id %d esta en la lista de LOCALIZED", id_localized);
			list_remove(lista_ids_localized, i);
			pthread_mutex_unlock(&mutex_lista_ids_localized);
			return 1;
		}
	}
>>>>>>> develop

	pthread_mutex_unlock(&mutex_lista_ids_localized);
	return 0;
}

<<<<<<< HEAD
//FIN DE CONEXIONES
//
//
//
=======
void esperar_mensaje_caught() {
	suscribirse_a_cola(conexion_caught, CAUGHT);
	while(funciona_broker == 1) {
		log_info(nuestro_log, "Estoy esperando mensaje CAUGHT");
		mensaje_broker* msj_broker = recibir_msj_broker(conexion_caught);
		if(msj_broker == NULL) {
			log_info(nuestro_log, "Se perdio la conexion con el broker");
		} else {
			int posicion_en_lista = obtener_posicion_en_lista_de_id_caught(msj_broker->id_correlativo);
			if(posicion_en_lista != -1) {
				log_info(nuestro_log, "El mensaje CAUGHT corresponde a un ID que se estaba esperando");
				pthread_mutex_lock(&mutex_lista_ids_caught);
				id_y_entrenador* iye = list_remove(lista_ids_caught, posicion_en_lista);
				pthread_mutex_unlock(&mutex_lista_ids_caught);

				int respuesta_caught;
				memcpy(&respuesta_caught, msj_broker->payload, sizeof(uint32_t));

				if(respuesta_caught == 1) {
					manejar_la_captura_del_pokemon(iye->entrenador);
				} else {
					manejar_la_no_captura_del_pokemon(iye->entrenador);
				}

				free(iye);
			}

			free(msj_broker->payload);
			free(msj_broker);
		}
	}
}

int obtener_posicion_en_lista_de_id_caught(int id_caught) {
	pthread_mutex_lock(&mutex_lista_ids_caught);

	for(int i = 0; i<list_size(lista_ids_caught); i++) {
		id_y_entrenador* iye = list_get(lista_ids_caught, i);
		if(iye->id == id_caught) {
			log_info(nuestro_log, "El id %d estan en la lista CAUGHT", iye->id);
			pthread_mutex_unlock(&mutex_lista_ids_caught);
			return i;

		}
	}
	pthread_mutex_unlock(&mutex_lista_ids_caught);
	return -1;
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

	free(a_enviar);
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
>>>>>>> develop

void cambiar_estado_entrenador(entrenador* entrenador,estado_entrenador un_estado){
	entrenador->estado = un_estado;
}

//Manejo la llegada de un nuevo pokemon (LOCALIZED O APPEARED)
void manejar_aparicion_de_pokemon(char* nombre, int posicion_x, int posicion_y) {
<<<<<<< HEAD
	if(el_pokemon_es_requerido(nombre)) {
		pokemon* nuevo_pokemon  = malloc(sizeof(pokemon));
		nuevo_pokemon->nombre = nombre;
		nuevo_pokemon->posicion = armar_posicion(string_from_format("%d|%d", posicion_x, posicion_y));

		buscar_entrenador_a_planificar_para_moverse(nuevo_pokemon);
	}
}

void entrenador_disponible(entrenador* entrenador){

	cambiar_estado_entrenador(entrenador, READY);

	if(queue_size(pokemons_sin_entrenador) > 0){
		log_info(nuestro_log,"Un entrenador que se libero esta atendiendo a un nuevo pokemon");
		pokemon* pokemon = queue_pop(pokemons_sin_entrenador);
		agregar_entrenador_a_entrenadores_ready(entrenador, pokemon);
	}
}

void buscar_entrenador_a_planificar_para_moverse(pokemon* pokemon_objetivo){
	//Seteo la variable global del utils para poder manejarla en los distintos metodos que me filtran al entrenador mas cerca
	pokemon_para_planificar = pokemon_objetivo;
	//Filtro entrenadores en estado NEW o BLOCK_READY, luego ordeno la lista para obtener al primero mas cercano y despues lo agrego a la lista de entredores ready
	t_list* entrenadores_mas_cercanos = list_sorted(list_filter(entrenadores, el_entrenador_se_puede_planificar), el_entrenador1_esta_mas_cerca);
	if(list_size(entrenadores_mas_cercanos) == 0) {
		queue_push(pokemons_sin_entrenador, pokemon_objetivo);
		log_info(nuestro_log,"NO HAY ENTRENADORES DISPONIBLES EN ESTE MOMENTO");
		//VER DESPUES ESTE CASO
	} else {
		agregar_entrenador_a_entrenadores_ready(list_get(entrenadores_mas_cercanos, 0), pokemon_objetivo);
	}
}

void agregar_entrenador_a_entrenadores_ready(entrenador* entrenador_listo, pokemon* pokemon_suelto) {

	entrenador_listo->pokemon_en_busqueda = pokemon_suelto;
	agregar_movimientos_en_x(entrenador_listo);
	agregar_movimientos_en_y(entrenador_listo);

	cambiar_estado_entrenador(entrenador_listo, READY);
	list_add(entrenadores_ready, entrenador_listo);
	pthread_mutex_unlock(&lock_de_planificacion);
}

void agregar_movimientos_en_x(entrenador* entrenador_listo) {
	int diferencia_en_x = diferencia_en_x_del_entrenador_al_pokemon(entrenador_listo, entrenador_listo->pokemon_en_busqueda);
=======
	//dictionary_has_key(objetivo_global,nombre_pokemon) && necesito_mas_de_ese_pokemon(nombre_pokemon)
	//el_pokemon_es_requerido(nombre)
	if(dictionary_has_key(objetivo_global,nombre)) {
		if(necesito_mas_de_ese_pokemon(nombre)){
			log_info(nuestro_log, "Aparecio un %s en %d|%d, el cual es requerido", nombre, posicion_x, posicion_y);

			//Se hace aca para que dos entrenadores no esten buscando al mismo pokemon
			restar_adquirido_a_objetivo_global(nombre);

			pokemon* nuevo_pokemon  = malloc(sizeof(pokemon));
			nuevo_pokemon->nombre = nombre;
			char* pos = string_from_format("%d|%d", posicion_x, posicion_y);
			nuevo_pokemon->posicion = armar_posicion(pos);
			free(pos);

			buscar_entrenador_a_planificar_para_moverse(nuevo_pokemon);
		} else {
			log_info(nuestro_log, "El pokemon %s no es requerido actualmente, pero podría serlo en un futuro", nombre);
			//Si no lo necesito actualmente, lo agrego a una lista de pokemons que podrian ser llamados
			pokemon* nuevo_pokemon  = malloc(sizeof(pokemon));
			nuevo_pokemon->nombre = nombre;
			char* pos = string_from_format("%d|%d", posicion_x, posicion_y);
			nuevo_pokemon->posicion = armar_posicion(pos);
			free(pos);

			agregar_pokemon_a_pokemons_en_espera(nuevo_pokemon);
		}
	} else {
		log_info(nuestro_log, "El pokemon %s no es requerido", nombre);
		free(nombre);
	}
}

void agregar_pokemon_a_pokemons_sin_entrenador(pokemon* nuevo_pokemon) {
	pthread_mutex_lock(&mutex_pokemons_sin_entrenador);
	queue_push(pokemons_sin_entrenador, nuevo_pokemon);
	pthread_mutex_unlock(&mutex_pokemons_sin_entrenador);
}

pokemon* quitar_pokemon_de_pokemons_sin_entrenador() {
	pthread_mutex_lock(&mutex_pokemons_sin_entrenador);
	pokemon * p = queue_pop(pokemons_sin_entrenador);
	pthread_mutex_unlock(&mutex_pokemons_sin_entrenador);

	return p;
}

void agregar_pokemon_a_pokemons_en_espera(pokemon* nuevo_pokemon) {
	pthread_mutex_lock(&mutex_pokemons_en_espera);
	list_add(pokemons_en_espera, nuevo_pokemon);
	pthread_mutex_unlock(&mutex_pokemons_en_espera);
}

pokemon* quitar_pokemon_de_pokemons_en_espera(int posicion) {
	pthread_mutex_lock(&mutex_pokemons_en_espera);
	pokemon * p = list_remove(pokemons_en_espera, posicion);
	pthread_mutex_unlock(&mutex_pokemons_en_espera);
	return p;
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
			buscar_entrenador_a_planificar_para_moverse(quitar_pokemon_de_pokemons_sin_entrenador());
		}
	}
}

void buscar_entrenador_a_planificar_para_moverse(pokemon* pokemon_objetivo){
	pthread_mutex_lock(&mutex_entrenadores);
	pthread_mutex_lock(&mutex_pokemon_para_planificar);
	//Seteo la variable global del utils para poder manejarla en los distintos metodos que me filtran al entrenador mas cerca
	pokemon_para_planificar = pokemon_objetivo;
	//Filtro entrenadores en estado NEW o BLOCK_READY, luego ordeno la lista para obtener al primero mas cercano y despues lo agrego a la lista de entredores ready
	t_list* entrenadores_para_planificar = list_filter(entrenadores, el_entrenador_se_puede_planificar);
	t_list* entrenadores_mas_cercanos = list_sorted(entrenadores_para_planificar, el_entrenador1_esta_mas_cerca);
	pthread_mutex_unlock(&mutex_pokemon_para_planificar);

	if(list_size(entrenadores_mas_cercanos) == 0) {
		agregar_pokemon_a_pokemons_sin_entrenador(pokemon_objetivo);
		log_info(nuestro_log,"No hay entrenadores disponibles en este momento, cuando alguno finalice el hilo de entrenadores disponibles se encargara de la asignacion.");
	} else {
		log_info(nuestro_log, "Se encontro entrenador para asignar a la busqueda de %s", pokemon_objetivo->nombre);
		agregar_entrenador_a_entrenadores_ready(list_get(entrenadores_mas_cercanos, 0), pokemon_objetivo);
	}
	list_destroy(entrenadores_para_planificar);
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
>>>>>>> develop
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

<<<<<<< HEAD
void agregar_movimientos_en_y(entrenador* entrenador_listo) {
	int diferencia_en_y = diferencia_en_y_del_entrenador_al_pokemon(entrenador_listo, entrenador_listo->pokemon_en_busqueda);
=======
void agregar_movimientos_en_y(entrenador* entrenador_listo, int diferencia_en_y) {
>>>>>>> develop
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
<<<<<<< HEAD

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
=======

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
>>>>>>> develop
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
		//Llegado a este punto se considera que realmente fue un DEADLOCK entre mas de un entrador (Por lo que se considera como DEADLOCK atacado)
		sumar_uno_a_cantidad_de_deadlocks();

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
		free(un_intercambio);
	}

	log_info(nuestro_log,"Sali de planear el intercambio");

}

void sumar_uno_a_cantidad_de_deadlocks() {
	pthread_mutex_lock(&mutex_cantidad_de_deadlocks);
	cantidad_de_deadlocks++;
	pthread_mutex_unlock(&mutex_cantidad_de_deadlocks);
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

		un_intercambio->pokemon1 = pokemon_que_sirven_para_intercambiar(entrenador1,entrenador_a_intercambiar);
		//EN POKEMON2 VOY A PONER A POKEMON SOBRANTE DEL ENTRENADOR1
		un_intercambio->pokemon2 = list_get(entrenador1->pokemons_sobrantes,0);

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
			char* pokemon1 = list_get(entrenador1->pokemons_objetivo,i);
			char* pokemon2 = list_get(entrenador_a_evaluar->pokemons_sobrantes,j);
			if(string_equals_ignore_case(pokemon1,pokemon2)){
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

	//TODO revisar caso en el que no entre al if (preguntar a Fede)
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
	return list_filter(entrenadores, el_entrenador_esta_block_deadlock);
}

int el_entrenador_esta_en_exit(entrenador* entrenador){
	return entrenador->estado == EXIT;
}

int terminaron_todos_los_entrenadores(){
	return list_all_satisfy(entrenadores, el_entrenador_esta_en_exit);
}

int el_entrenador_no_puede_capturar_mas_pokemons(entrenador* entrenador){
	return (list_size(entrenador->pokemons_adquiridos)) >= entrenador->cant_maxima_pokemons;
}

//Realiza un intercambio entre dos entrenadores que estaban bloqueados
void intercambiar(entrenador* entrenador1) {
	log_info(nuestro_log,"Empezando la accion de intercambio");
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


	log_info(nuestro_log, "Entrenador %d dando %s", entrenador1->id, pokemon_a_eliminar_en_1);
	log_info(nuestro_log, "Entrenador %d dando %s", intercambio_a_realizar->entrenador2->id, pokemon_a_eliminar_en_2);

	agregar_pokemon_a_adquirido(entrenador1, pokemon_a_eliminar_en_2);
	agregar_pokemon_a_adquirido(intercambio_a_realizar->entrenador2, pokemon_a_eliminar_en_1);

	accionar_en_funcion_del_estado_del_entrenador(entrenador1);
	accionar_en_funcion_del_estado_del_entrenador(intercambio_a_realizar->entrenador2);

	destruir_intercambio(intercambio_a_realizar);

	log_info(nuestro_log,"Terminando la accion de intercambio");
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
			int largo_key = strlen(key);
			buffer->tamanio = largo_key + 1 + sizeof(uint32_t);
			void* stream = malloc(buffer->tamanio);
			int offset = 0;
			memcpy(stream + offset,&largo_key, sizeof(uint32_t));
			offset+=sizeof(uint32_t);
			memcpy(stream + offset,key, (largo_key + 1));
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
		log_info(nuestro_log, "Se recibio correctamente el ID: %d, para esperar en LOCALIZED", id_localized);

		pthread_mutex_lock(&mutex_lista_ids_localized);
		list_add(lista_ids_localized, id_localized);
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
		log_info(nuestro_log, "9. Se realizará el CATCH por DEFAULT debido a que la conexion con el broker fallo.");
		manejar_la_captura_del_pokemon(entrenador);
	} else {
		//ACCION CON EL BROKER

		//CREO EL BUFFER CON SU TAMANIO Y STREAM
		t_buffer* buffer = malloc(sizeof(t_buffer));

		int tamanio_char_pokemon = strlen(entrenador->pokemon_en_busqueda->nombre);
		buffer->tamanio = tamanio_char_pokemon + 1 + (3*sizeof(uint32_t));

		void* stream = malloc(buffer->tamanio);
		int offset = 0;
		memcpy(stream + offset,&tamanio_char_pokemon, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset,entrenador->pokemon_en_busqueda->nombre, (tamanio_char_pokemon+1));
		offset += (tamanio_char_pokemon+1);
		memcpy(stream + offset,&(entrenador->pokemon_en_busqueda->posicion->posicion_x), sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset,&(entrenador->pokemon_en_busqueda->posicion->posicion_y), sizeof(uint32_t));

		buffer->stream = stream;

		//CREO EL PAQUETE CON EL CONTENIDO DE LO QUE VOY A ENVIAR
		t_paquete* paquete = crear_paquete(MENSAJE, CATCH, buffer);

		//SERIALIZO EL PAQUETE, LO MANDO y ESPERO LA RESPUESTA DEL BROKER
		int bytes = obtener_tamanio_de_paquete(paquete);
		void* a_enviar = serializar_paquete(paquete, bytes);

		if(send(socket_catch, a_enviar, bytes ,0) > 0){
			log_info(nuestro_log, "Se realizo el envio de CATCH %s correctamente", entrenador->pokemon_en_busqueda->nombre);

			socket_y_entrenador* s_y_e = malloc(sizeof(socket_y_entrenador));
			s_y_e->conexion = socket_catch;
			s_y_e->entrenador = entrenador;

			pthread_t* hilo_espera_catch;
			pthread_create(&hilo_espera_catch,NULL, esperar_id_caught, s_y_e);
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

void esperar_id_caught(socket_y_entrenador* sye) {
	int id_caught;

	int conexion = sye->conexion;
	entrenador* entr = sye->entrenador;
	free(sye);

	if(recv(conexion, &id_caught, sizeof(int), 0) > 0){
		log_info(nuestro_log, "Se recibio correctamente el ID: %d, para esperar en CAUGHT", id_caught);

		id_y_entrenador* iye = malloc(sizeof(id_y_entrenador));
		iye->id = id_caught;
		iye->entrenador = entr;

		pthread_mutex_lock(&mutex_lista_ids_caught);
		list_add(lista_ids_caught, iye);
		pthread_mutex_unlock(&mutex_lista_ids_caught);
	} else {
		log_info(logger, "9. No se pudo recibir el ID de CAUGHT, se realizará el CATCH por DEFAULT");
		log_info(nuestro_log, "9. No se pudo recibir el ID de CAUGHT, se realizará el CATCH por DEFAULT");
		manejar_la_captura_del_pokemon(entr);
	}
	liberar_conexion(conexion);
}
void accionar_en_funcion_del_estado_del_entrenador(entrenador* entrenador){

	if(el_entrenador_cumplio_su_objetivo(entrenador)) {
		log_info(nuestro_log,"El entrenador %d cumplio su objetivo y queda en estado EXIT", entrenador->id);
		cambiar_estado_entrenador(entrenador, EXIT);
	} else if (el_entrenador_no_puede_capturar_mas_pokemons(entrenador)) {
		log_info(nuestro_log,"El entrenador %d se bloquea quedando en estado DEADLOCK", entrenador->id);
		cambiar_estado_entrenador(entrenador, BLOCK_DEADLOCK);
	} else {
		log_info(nuestro_log,"El entrenador %d queda en BLOCK_READY", entrenador->id);
		cambiar_estado_entrenador(entrenador, BLOCK_READY);
	}
}

void manejar_la_no_captura_del_pokemon(entrenador* entrenador) {
	pokemon* pokemon_en_captura = entrenador->pokemon_en_busqueda;
	log_info(nuestro_log, "3. No se pudo realizar la captura del pokemon %s, en la posicion %d|%d exitosamente.", pokemon_en_captura->nombre, pokemon_en_captura->posicion->posicion_x, pokemon_en_captura->posicion->posicion_y);

	agregar_objetivo_a_objetivo_global(pokemon_en_captura->nombre);

	free(pokemon_en_captura->nombre);
	destruir_pokemon(pokemon_en_captura);

	accionar_en_funcion_del_estado_del_entrenador(entrenador);
}

void manejar_la_captura_del_pokemon(entrenador* entrenador) {
	pokemon* pokemon_en_captura = entrenador->pokemon_en_busqueda;
	log_info(logger, "3. Se realiza la captura del pokemon %s, en la posicion %d|%d exitosamente.", pokemon_en_captura->nombre, pokemon_en_captura->posicion->posicion_x, pokemon_en_captura->posicion->posicion_y);
	log_info(nuestro_log, "3. Se realiza la captura del pokemon %s, en la posicion %d|%d exitosamente.", pokemon_en_captura->nombre, pokemon_en_captura->posicion->posicion_x, pokemon_en_captura->posicion->posicion_y);


	//restar_adquirido_a_objetivo_global(pokemon_en_captura->nombre); ESTO SE HACE CUANDO SE VA A BUSCAR AL POKEMON
	agregar_pokemon_a_adquirido(entrenador, pokemon_en_captura->nombre);

	evaluar_pokemons_en_espera(pokemon_en_captura->nombre);

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

void evaluar_pokemons_en_espera(char* nombre) {
	pthread_mutex_lock(&mutex_pokemons_en_espera);

	for(int i = 0; i<list_size(pokemons_en_espera); i++) {
		pokemon* p = list_get(pokemons_en_espera, i);
		if(string_equals_ignore_case(nombre, p->nombre)) {
			p = quitar_pokemon_de_pokemons_en_espera(i);
			agregar_pokemon_a_pokemons_sin_entrenador(p);
			pthread_mutex_unlock(&lock_de_entrenador_disponible);
			break;
		}
	}

	pthread_mutex_unlock(&mutex_pokemons_en_espera);
}


void mostrar_metricas(t_log* log) {
	log_info(log, "-------- METRICAS --------");
	log_info(log, "Cantidad de ciclos de CPU total: %d", calcular_ciclos_de_CPU_totales());
	//TODO ver a que se considera cambio de contexto
	log_info(log, "Cantidad de cambios de contextos realizados: %d", cantidad_de_cambios_de_contexto);
	mostrar_ciclos_de_CPU_por_entrenador(log);
	log_info(log, "Deadlocks producidos y resueltos: %d", cantidad_de_deadlocks);
}

int calcular_ciclos_de_CPU_totales() {
	int acum = 0;
	for(int i = 0; i<list_size(entrenadores); i++) {
		entrenador* e = list_get(entrenadores, i);
		acum += e->cpu_usado;
	}
	return acum;
}

void mostrar_ciclos_de_CPU_por_entrenador(t_log* log) {
	for(int i = 0; i<list_size(entrenadores); i++) {
		entrenador* e = list_get(entrenadores, i);
		log_info(log, "Cantidad de ciclos de CPU realizados por entrenador %d: %d", e->id, e->cpu_usado);
	}
}
