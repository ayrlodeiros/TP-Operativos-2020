#include "broker-v2.h"


struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

int main(void)
{

	iniciar_broker();

	levantar_servidor(leer_ip_broker(),leer_puerto_broker());

	terminar_broker();

	return EXIT_SUCCESS;

}

void iniciar_broker(){

	iniciar_config();
	logger = log_create(leer_log_file(),"broker",false,LOG_LEVEL_INFO);
	mi_log = log_create(leer_nuestro_log_file(),"broker",true,LOG_LEVEL_INFO);
	dump = log_create(leer_path_archivo_dump(),"dump",false,LOG_LEVEL_INFO);
	iniciar_funcionalidades();
	printf("-----------------BROKER LEVANTADOr-----------------\n");
}

void terminar_broker(){
	destruir_config();
	liberar_message_queues();
	log_destroy(logger);
	log_destroy(logger);
	log_destroy(dump);
	printf("-----------------BROKER CERRADO-----------------\n");
}
