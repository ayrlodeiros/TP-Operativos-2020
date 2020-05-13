#include "gameboy-v2.h"

int main(void)
{
	iniciar_gameBoy();
	//int socket_broker = crear_conexion_del_cliente(leer_ip_broker(),4444,logger);

	terminar_gameBoy();

	return EXIT_SUCCESS;
}


void iniciar_gameBoy(){
	iniciar_config();
	logger = log_create("src/resources/log_gameBoy.txt","gameBoy",false,LOG_LEVEL_INFO);
	mi_log = log_create("src/resources/mi_log_gameBoy.txt","gameBoy",true,LOG_LEVEL_INFO);
	log_info(mi_log, "-----------------Leyendo archivo config-----------------");
	log_info(mi_log, leer_ip_broker());
	log_info(mi_log, leer_ip_team());
	log_info(mi_log, leer_ip_gamecard());
	log_info(mi_log, leer_puerto_broker());
	log_info(mi_log, leer_puerto_team());
	log_info(mi_log, leer_puerto_gamecard());
}

void terminar_gameBoy(){
	log_info(mi_log, "-----------------Destruyendo archivo config-----------------");
	destruir_config();
	log_destroy(logger);
	log_destroy(mi_log);
}
