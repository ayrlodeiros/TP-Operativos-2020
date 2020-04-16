#include "servidor.h"

int main(void)
{
	iniciar_logger();
	iniciar_config();

	iniciar_servidor();

	destruir_logger();
	destruir_config();

	return EXIT_SUCCESS;
}
