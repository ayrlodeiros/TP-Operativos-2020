#include "gameboy-v2.h"

int main(void)
{
	iniciar_gameBoy();
	int socket_broker = crear_conexion_del_cliente("127.0.0.2","5002",logger);

	log_info(mi_log, "-----------------Leyendo archivo config-----------------");
	log_info(mi_log, leer_ip_broker());
	log_info(mi_log, leer_ip_team());
	log_info(mi_log, leer_ip_gamecard());
	//log_info(mi_log,("%s",leer_puerto_broker()));
	//log_info(mi_log, leer_puerto_team());
	//log_info(mi_log, leer_puerto_gamecard());

	terminar_gameBoy();

	return EXIT_SUCCESS;
}


void iniciar_gameBoy(){
	iniciar_config();
	logger = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gameboy-v2/src/resources/log_gameBoy.txt","gameBoy",false,LOG_LEVEL_INFO);
	mi_log = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gameboy-v2/src/resources/mi_log_gameBoy.txt","gameBoy",true,LOG_LEVEL_INFO);
}

void terminar_gameBoy(){
	log_info(mi_log, "-----------------Destruyendo archivo config-----------------");
	destruir_config();
	log_destroy(logger);
	log_destroy(mi_log);
}
