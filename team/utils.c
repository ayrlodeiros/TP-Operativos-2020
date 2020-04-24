#include"utils.h"

void conectar_broker(void) {
	while(1) {
		log_info(nuestro_log, "Conectando con broker...");
		sleep(leer_tiempo_reconexion());
	}
}

