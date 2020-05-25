
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

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		log_error(logger, string_from_format("El cliente fallo al conectarse en IP: %s y PUERTO: %s", ip, puerto));

	freeaddrinfo(server_info);
	log_info(logger, string_from_format("El cliente se conecto exitosamente en IP: %s y PUERTO: %s", ip, puerto));
	return socket_cliente;
}

void liberar_conexion(int socket) {
	close(socket);
}

void enviar_mensaje_appeared(t_appeared_pokemon appeared_pokemon, int socket_team){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = APPEARED_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(&appeared_pokemon.nombre_pokemon) + 1 + sizeof(uint32_t) * 3;

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &appeared_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &appeared_pokemon.nombre_pokemon, strlen(appeared_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(appeared_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &appeared_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &appeared_pokemon.posicionY, sizeof(uint32_t));

	paquete->buffer->stream = stream;

	int size_serializados;
	void *mensaje_a_enviar = serializar_paquete(paquete,&size_serializados);

	send(socket_team,mensaje_a_enviar,size_serializados,0);


	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}



void* serializar_paquete(t_paquete* paquete, int *bytes){
	int size_serializado = sizeof(paquete->codigo_operacion) + sizeof(paquete->buffer->size) + (*bytes);
	void* buffer = malloc(size_serializado);

	int bytes_escritos = 0;

	memcpy(buffer + bytes_escritos, &paquete->codigo_operacion, sizeof(paquete->codigo_operacion));
	bytes_escritos += sizeof(paquete->codigo_operacion);

	memcpy(buffer + bytes_escritos, &paquete->buffer->size, sizeof(int));
	bytes_escritos += sizeof(int);

	memcpy(buffer + bytes_escritos, &paquete->buffer->stream, paquete->buffer->size);
	bytes_escritos += paquete->buffer->size;

	(*bytes) = size_serializado;
	return buffer;
}
