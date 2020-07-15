#include "broker-v2.h"


struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

void while_infinito() {
	while(1){}
}

bool esMayorA3(void* elemento){
	return strlen(((char*) elemento)) > 3;
}


int main(void)
{
	//pthread_t* conexion_team;
	t_list* lista = list_create();
	char* uno = "Uno";
	char* dos = "Dos";
	char* tres = "Tres";
	char* cuatro = "Cuatro";

	list_add(lista,uno);
	list_add(lista,dos);
	list_add(lista,tres);
	list_add(lista,cuatro);
	printf("Elemento 1 de la primera lista es %s\n",list_get(lista,0));
	printf("Elemento 2 de la primera lista es %s\n",list_get(lista,1));
	printf("Elemento 3 de la primera lista es %s\n",list_get(lista,2));
	printf("Elemento 4 de la primera lista es %s\n",list_get(lista,3));
	printf("Elemento 5 de la primera lista es %s\n",list_get(lista,4));

	t_list* lista_nueva = list_filter(lista,esMayorA3);

	printf("Elemento 1 de la primera lista es %s\n",list_get(lista,0));
	printf("Elemento 2 de la primera lista es %s\n",list_get(lista,1));
	printf("Elemento 3 de la primera lista es %s\n",list_get(lista,2));
	printf("Elemento 4 de la primera lista es %s\n",list_get(lista,3));
	list_destroy_and_destroy_elements(lista,free);

	printf("Elemento 1 de la nueva lista es %s\n",list_get(lista_nueva,0));
	printf("Elemento 2 de la nueva lista es %s\n",list_get(lista_nueva,1));

	/*
	iniciar_broker();

	levantar_servidor(leer_ip_broker(),leer_puerto_broker());

	terminar_broker();

	//pthread_t* hilo_infinito;
*/
	return EXIT_SUCCESS;

}

void iniciar_broker(){

	printf("-----------------Iniciando archivo config-----------------\n");
	logger = log_create(leer_log_file(),"broker",false,LOG_LEVEL_INFO);
	mi_log = log_create(leer_nuestro_log_file(),"broker",true,LOG_LEVEL_INFO);
	iniciar_funcionalidades();

}

void terminar_broker(){
	printf("-----------------Destruyendo archivo config-----------------\n");
	destruir_config();
	liberar_message_queues();
	log_destroy(logger);
	log_destroy(mi_log);
}
