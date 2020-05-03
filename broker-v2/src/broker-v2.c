#include "broker-v2.h"


int main(void)
{
	printf("Hola");


	//iniciar_broker();

	//terminar_broker();

	return EXIT_SUCCESS;
}

void iniciar_broker(){
	iniciar_config();
	logger = log_create(leer_log_file(),"broker",false,LOG_LEVEL_INFO);
	mi_log = log_create("src/resources/mi_log_broker.txt","broker",true,LOG_LEVEL_INFO);
}

void terminar_broker(){
	destruir_config();
	log_destroy(logger);
	log_destroy(mi_log);
}
