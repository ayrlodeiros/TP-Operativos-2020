
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

void levantar_servidor(char* ip, int port, t_log* logger) {

	int socket_servidor;
	char* puerto = string_itoa(port);
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

	log_info(logger, string_from_format("Servidor levantado en IP: %s y PUERTO: %s", ip, puerto));

	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);

	printf("Esperando conexiones\n");
	pthread_t* prueba;

	while(1)
	esperar_cliente(socket_servidor,logger);
}

void esperar_cliente(int socket_servidor,t_log* logger)
{
	int err;

	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	log_info(mi_log,"Estableci una conexion con un modulo de socket\n");
	log_info(logger,"Estableci una conexion con un modulo\n");

	err = pthread_create(&espera,NULL,(void*)servir_cliente,&socket_cliente);
	if( err != 0){
		log_info(logger,string_from_format("Hubo un error al intentar crear el thread: %s",strerror(err)));
	}
	pthread_detach(espera);


}

void servir_cliente(int* socket)
{
	int cod_op;
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request(cod_op, *socket);
}

void process_request(int cod_op, int socket_cliente) {
	int cola_code;
	recv(socket_cliente,&cola_code,sizeof(int),MSG_WAITALL);
	int modulo;
	switch (cod_op) {
		case MENSAJE:
			/** Por ahora para probar uso solo una cola*/
			switch_mensaje(cola_code,socket_cliente);
			break;
		case SUSCRIPCION:
			/**Faltaria definir bien cuando se recibe cada cosa, por ahora lo pongo asi */
			recv(socket_cliente,&modulo,sizeof(int),MSG_WAITALL);
			switch_suscripcion(cola_code,socket_cliente,modulo);
			break;
		case -1:
			log_info(mi_log,"HUbo un error al tratar de recibir el mensaje\n");
			pthread_exit(NULL);
		}
}
