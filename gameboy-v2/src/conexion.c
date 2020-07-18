
#include "conexion.h"


int crear_conexion_del_cliente(char *ip, char* puerto, t_log* logger) {
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
		log_error(logger, "El cliente fallo al conectarse en IP: %s y PUERTO: %s", ip, puerto);
		log_error(mi_log, "El cliente fallo al conectarse en IP: %s y PUERTO: %s", ip, puerto);
	}else{
		log_info(logger, "El cliente se conecto exitosamente en IP: %s y PUERTO: %s", ip, puerto);
		log_info(mi_log, "El cliente se conecto exitosamente en IP: %s y PUERTO: %s", ip, puerto);
	}
	freeaddrinfo(server_info);
	free(puerto);
	return socket_cliente;
}

void liberar_conexion(int socket) {
	close(socket);
}

void enviar_mensaje_appeared_broker(t_appeared_pokemon appeared_pokemon, int socket, uint32_t id_mensaje_correlativo){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = APPEARED_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(appeared_pokemon.nombre_pokemon) + 1 + sizeof(uint32_t) * 3;

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &appeared_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, appeared_pokemon.nombre_pokemon, strlen(appeared_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(appeared_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &appeared_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &appeared_pokemon.posicionY, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 5*sizeof(uint32_t);

	void *mensaje_a_enviar = malloc(size_serializados);
	mensaje_a_enviar = serializar_paquete_con_id(paquete,size_serializados,id_mensaje_correlativo);

	//log_info(mi_log,string_from_format("sizes: %d",size_serializados));

	if(send(socket,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, "Se envio el mensaje APPEARED: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker());
		log_info(mi_log, "Se envio el mensaje APPEARED: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker());
	}else{
		log_error(logger, "No se pudo enviar el mensaje APPEARED.");
		log_error(mi_log, "No se pudo enviar el mensaje APPEARED.");
	}

	free(mensaje_a_enviar); //DONDE PIDO LA MEMORIA!!
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje_appeared_team(t_appeared_pokemon appeared_pokemon, int socket){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = APPEARED_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(appeared_pokemon.nombre_pokemon) + 1 + sizeof(uint32_t) * 3;
	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &appeared_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, appeared_pokemon.nombre_pokemon, strlen(appeared_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(appeared_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &appeared_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &appeared_pokemon.posicionY, sizeof(uint32_t));

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 4*sizeof(int);

	//void *mensaje_a_enviar = serializar_paquete(paquete,size_serializados);
	void *mensaje_a_enviar = malloc(size_serializados);
	mensaje_a_enviar = serializar_paquete(paquete,size_serializados);

	//log_info(mi_log,string_from_format("sizes: %d",size_serializados));

	if(send(socket,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, "Se envio el mensaje APPEARED: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker());
		log_info(mi_log, "Se envio el mensaje APPEARED: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker());
	}else{
		log_error(logger, "No se pudo enviar el mensaje APPEARED.");
		log_error(mi_log, "No se pudo enviar el mensaje APPEARED.");
	}

	free(mensaje_a_enviar); //DONDE PIDO LA MEMORIA!!
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
void enviar_mensaje_new_broker(t_new_pokemon new_pokemon, int socket){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = NEW_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = new_pokemon.largo_nombre_pokemon + 1 + sizeof(uint32_t) * 4;

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &new_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, new_pokemon.nombre_pokemon, strlen(new_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(new_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &new_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &new_pokemon.posicionY, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &new_pokemon.cantidad_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 5*sizeof(uint32_t);

	void *mensaje_a_enviar = serializar_paquete_con_id(paquete,size_serializados,-1);

	if(send(socket,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, "Se envio el mensaje NEW: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker);
	}else{
		log_error(logger, "No se pudo enviar el mensaje NEW.");
	}

	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void enviar_mensaje_new_gamecard(t_new_pokemon new_pokemon, int socket, int id_mensaje){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = NEW_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = new_pokemon.largo_nombre_pokemon + 1 + sizeof(uint32_t) * 4;

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &new_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, new_pokemon.nombre_pokemon, strlen(new_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(new_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &new_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &new_pokemon.posicionY, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &new_pokemon.cantidad_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 5*sizeof(uint32_t);

	void *mensaje_a_enviar = serializar_paquete_con_id(paquete,size_serializados,id_mensaje);

	if(send(socket,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, "Se envio el mensaje NEW: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker);
	}else{
		log_error(logger, "No se pudo enviar el mensaje NEW.");
	}

	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void enviar_mensaje_catch_gamecard(t_catch_pokemon catch_pokemon, int socket, uint32_t id_mensaje){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = CATCH_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(catch_pokemon.nombre_pokemon) + 1 + sizeof(uint32_t) * 3;

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &catch_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, catch_pokemon.nombre_pokemon, strlen(catch_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(catch_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &catch_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &catch_pokemon.posicionY, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 5*sizeof(uint32_t);

	void *mensaje_a_enviar = serializar_paquete_con_id(paquete,size_serializados,id_mensaje);

	if(send(socket,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, "Se envio el mensaje CATCH: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker);
	}else{
		log_error(logger, "No se pudo enviar el mensaje CATCH.");
	}


	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void enviar_mensaje_catch_broker(t_catch_pokemon catch_pokemon, int socket){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = CATCH_POKEMON;


	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(catch_pokemon.nombre_pokemon) + 1 + sizeof(uint32_t) * 3;

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &catch_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, catch_pokemon.nombre_pokemon, strlen(catch_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(catch_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &catch_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &catch_pokemon.posicionY, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 5*sizeof(uint32_t);

	void *mensaje_a_enviar = serializar_paquete_con_id(paquete,size_serializados,-1);

	if(send(socket,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, "Se envio el mensaje CATCH: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker);
	}else{
		log_error(logger, "No se pudo enviar el mensaje CATCH.");
	}


	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void enviar_mensaje_caught(t_caught_pokemon caught_pokemon, int socket_broker,uint32_t id_mensaje_correlativo){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = CAUGHT_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = sizeof(uint32_t);

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &caught_pokemon.atrapado, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 5*sizeof(uint32_t);

	void *mensaje_a_enviar = serializar_paquete_con_id(paquete,size_serializados,id_mensaje_correlativo);
	log_info(mi_log,"sizes: %d",size_serializados);

	if(send(socket_broker,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, "Se envio el mensaje CAUGHT: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker());
	}else{
		log_error(logger, "No se pudo enviar el mensaje CAUGHT.");
	}

	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void enviar_mensaje_get_gamecard(t_get_pokemon get_pokemon, int socket, uint32_t id_mensaje){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = GET_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = get_pokemon.largo_nombre_pokemon + 1 + sizeof(uint32_t);

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;
	memcpy(stream + bytes_escritos, &get_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos,  get_pokemon.nombre_pokemon, strlen(get_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(get_pokemon.nombre_pokemon)+1;

	paquete->buffer->stream = stream;

	int size_serializados = paquete->buffer->size + 5*sizeof(uint32_t);

	void *mensaje_a_enviar = serializar_paquete_con_id(paquete,size_serializados, id_mensaje);
	int envio = send(socket,mensaje_a_enviar,size_serializados,0);

	if(envio > 0){
		log_info(logger, "Se envio el mensaje GET a IP: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker());
	}else{
		log_error(logger, "No se pudo enviar el mensaje GET.");
	}

	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje_get_broker(t_get_pokemon get_pokemon, int socket){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = GET_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = get_pokemon.largo_nombre_pokemon + 1 + sizeof(uint32_t);

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;
	memcpy(stream + bytes_escritos, &get_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos,  get_pokemon.nombre_pokemon, strlen(get_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(get_pokemon.nombre_pokemon)+1;

	paquete->buffer->stream = stream;

	int size_serializados = paquete->buffer->size + 5*sizeof(uint32_t);

	void *mensaje_a_enviar = serializar_paquete(paquete,size_serializados);
	int envio = send(socket,mensaje_a_enviar,size_serializados,0);

	if(envio > 0){
		log_info(logger, "Se envio el mensaje GET a IP: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker());
	}else{
		log_error(logger, "No se pudo enviar el mensaje GET.");
	}

	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

int suscribirse_a_cola(int cola, int tiempo, int socket_broker){

	int resultado = -1;

	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = SUSCRIPCION;
	paquete->mensaje = cola;

	int size_serializados = 3*sizeof(int);

	void *mensaje_a_enviar = serializar_paquete_sin_buffer(paquete,size_serializados);

	if(send(socket_broker,mensaje_a_enviar,size_serializados,0)>0){
		log_info(mi_log, "Se suscribio correctamente a la cola %d por %d segundos", cola, tiempo);
		log_info(logger, "Se suscribio correctamente a la cola %d por %d segundos", cola, tiempo);
		resultado = 1;
	}else{
		log_error(mi_log,"No se pudo suscribir a la cola %d",cola);
		log_error(logger,"No se pudo suscribir a la cola %d",cola);
		resultado = 0;
	}

	free(mensaje_a_enviar);
	free(paquete);

	return resultado;
}

void recibir_msj_broker(int conexion_broker) {
	int hubo_error = 0;
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
				payload = malloc(tamanio);
				if(recv(conexion_broker, payload, tamanio, 0) == -1) {
					hubo_error = 1;
				}
			}
		}
	}

	if(hubo_error) {
		log_info(logger, "No se pudo recibir el mensaje del broker, se perdio la conexion");
		log_info(mi_log, "No se pudo recibir el mensaje del broker, se perdio la conexion");
	} else {
		log_info(logger, "Se recibio un mensaje del broker", conexion_broker);
		log_info(logger, "ID: %d", id);
		log_info(logger, "ID CORRELATIVO: %d", id_cor);
		log_info(logger, "TAMANIO: %d", tamanio);
		log_info(mi_log, "Se recibio un mensaje del broker", conexion_broker);
		log_info(mi_log, "ID: %d", id);
		log_info(mi_log, "ID CORRELATIVO: %d", id_cor);
		log_info(mi_log, "TAMANIO: %d", tamanio);
		mandar_ack(conexion_broker, 1);
		free(payload);
	}
}

void mandar_ack(int conexion, int resultado) {
	send(conexion, &resultado, sizeof(int), 0);
}

void* serializar_paquete(t_paquete* paquete, int bytes){
	void* a_enviar = malloc(bytes);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->modulo),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->cod_op),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->mensaje),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->buffer->size),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

void* serializar_paquete_con_id(t_paquete* paquete, int bytes, int id){
	void* a_enviar = malloc(bytes);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->modulo),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->cod_op),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->mensaje),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&id,sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->buffer->size),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

void* serializar_paquete_sin_buffer(t_paquete* paquete, int tamanio_a_enviar){
	void * a_enviar = malloc(tamanio_a_enviar);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->modulo),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->cod_op),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset,&(paquete->mensaje),sizeof(uint32_t));

	return a_enviar;
}

char* leer_puerto_string(int modulo){
	switch (modulo) {
		case BROKER:
			return string_from_format("%d",leer_puerto_broker());
		case GAMECARD:
			return string_from_format("%d",leer_puerto_gamecard());
		case TEAM:
			return string_from_format("%d",leer_puerto_team());
		default:
			return "Modulo incorrecto";
	}
}

//agrego logs de conexion a los procesos
int conectarse_a(int modulo){
	switch (modulo) {
		case BROKER:
			//log_info(logger,string_from_format("SE ESTABLECIO CONEXION CON EL PROCESO: %s ", modulo));
			//log_info(mi_log,string_from_format("SE ESTABLECIO CONEXION CON EL PROCESO: %s ", modulo));
			//log_info(mi_log,"SE ESTABLECIO CONEXION CON EL PROCESO: %s", modulo);
			return crear_conexion_del_cliente(leer_ip_broker(),leer_puerto_string(BROKER),logger);
		case GAMECARD:
			//log_info(logger,"SE ESTABLECIO CONEXION CON EL PROCESO: %s", modulo);
			//log_info(mi_log,"SE ESTABLECIO CONEXION CON EL PROCESO: %s", modulo);
			return crear_conexion_del_cliente(leer_ip_gamecard(),leer_puerto_string(GAMECARD),logger);
		case TEAM:
			//log_info(logger,"SE ESTABLECIO CONEXION CON EL PROCESO: %s", modulo);
			//log_info(mi_log,"SE ESTABLECIO CONEXION CON EL PROCESO: %s", modulo);
			//log_info(mi_log,"1");
			return crear_conexion_del_cliente(leer_ip_team(),leer_puerto_string(TEAM),logger);
		default:
			return -1;
	}
}

