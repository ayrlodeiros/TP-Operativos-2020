#include "broker-v2.h"


int main(void)
{
	printf("Hola");

	return EXIT_SUCCESS;
}

void iniciar_broker(){
	iniciar_config();
	logger = log_create(leer_log_file,"broker",false,LOG_LEVEL_INFO);
	mi_log = log_create("src/resources/mi_log_broker.txt","broker",true,LOG_LEVEL_INFO);
}

void terminar_broker(){

}
