#include "broker-v2.h"


struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

int main(void)
{

	//pthread_t* conexion_team;

	iniciar_broker();

	levantar_servidor(leer_ip_broker(),leer_puerto_broker(),mi_log);

	terminar_broker();

	printf("Todo joya \n");


	return EXIT_SUCCESS;

}

void iniciar_broker(){
	printf("-----------------Iniciando archivo config-----------------\n");
	iniciar_config();
	inicializar_message_queues();
	logger = log_create(leer_log_file(),"broker",false,LOG_LEVEL_INFO);
	mi_log = log_create("src/resources/mi_log_broker.txt","broker",true,LOG_LEVEL_INFO);

}

void terminar_broker(){
	printf("-----------------Destruyendo archivo config-----------------\n");
	destruir_config();
	liberar_message_queues();
	log_destroy(logger);
	log_destroy(mi_log);
}
