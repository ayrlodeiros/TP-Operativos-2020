#include "broker.h"


int main(void)
{
	printf("Hola");

	return EXIT_SUCCESS;
}

void iniciar_broker(){
	logger = log_create("broker.log","broker",false,LOG_LEVEL_INFO);
}
