#include "gameboy-v2.h"

int main(void)
{
	iniciar_gameBoy();
	printf("1");
	int socket_broker = crear_conexion_del_cliente(leer_ip_broker(),4444,logger);

	terminar_gameBoy();

	return EXIT_SUCCESS;
}


void iniciar_gameBoy(){
	printf("-----------------Iniciando archivo config-----------------\n");
	iniciar_config();
	printf("2");
	logger = log_create("src/resources/log_gameBoy.txt","gameBoy",false,LOG_LEVEL_INFO);
	mi_log = log_create("src/resources/mi_log_gameBoy.txt","gameBoy",true,LOG_LEVEL_INFO);

}

void terminar_gameBoy(){
	printf("-----------------Destruyendo archivo config-----------------\n");
	destruir_config();
	log_destroy(logger);
	log_destroy(mi_log);
}
