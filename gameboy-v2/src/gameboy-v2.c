#include "gameboy-v2.h"

int main(int argc, char *argv[])
{
	iniciar_gameBoy();
	log_info(mi_log, string_from_format("cant argumentos: %d",argc));
	char* tipo_mensaje = argv[2];

	if(argc > 1){
		if(!strcmp(tipo_mensaje,"APPEARED_POKEMON")){
			gestionar_envio_appeared(argv);
		} else if(!strcmp(tipo_mensaje,"GET_POKEMON")){
			gestionar_envio_get(argv);
		}else if(!strcmp(tipo_mensaje,"NEW_POKEMON")){
			gestionar_envio_new(argv);
		}else if(!strcmp(tipo_mensaje,"CATCH_POKEMON")){
			gestionar_envio_catch(argv);
		}else if(!strcmp(tipo_mensaje,"CAUGHT_POKEMON")){
			gestionar_envio_caught(argv);
		}
		log_info(mi_log,"mensaje enviado");
	}

	/*int socket_prueba = conectarse_a(BROKER);
	t_get_pokemon *aget_pokemon = malloc (sizeof(t_get_pokemon));
	aget_pokemon->nombre_pokemon = "gonzalo miele";
	aget_pokemon->largo_nombre_pokemon = strlen(aget_pokemon->nombre_pokemon);
	log_info(mi_log,string_from_format("largo nombre: %d",   aget_pokemon->largo_nombre_pokemon));
	log_info(mi_log,string_from_format("nombre: %s",   aget_pokemon->nombre_pokemon));
	enviar_mensaje_get(*aget_pokemon, socket_prueba,leer_puerto_broker(), 100);
	log_info(mi_log,"volvi");
	free(aget_pokemon); */

	/*if(argc > 1){
		switch_envio_mensaje(*argv);
	}*/

	terminar_gameBoy();

	return EXIT_SUCCESS;
}


void iniciar_gameBoy(){
	iniciar_config();
	logger = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gameboy-v2/src/resources/log_gameBoy.txt","gameBoy",false,LOG_LEVEL_INFO);
	mi_log = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gameboy-v2/src/resources/mi_log_gameBoy.txt","gameBoy",true,LOG_LEVEL_INFO);
	log_info(mi_log, "-----------------Leyendo archivo config-----------------");
}

void terminar_gameBoy(){
	log_info(mi_log, "-----------------Destruyendo archivo config-----------------");
	destruir_config();
	log_destroy(logger);
	log_destroy(mi_log);
}

void gestionar_envio_appeared(char* argv[]){
	t_appeared_pokemon *appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	int socket;
	char* tipo_modulo = argv[1];

	appeared_pokemon->nombre_pokemon = argv[3];
	appeared_pokemon->largo_nombre_pokemon = strlen(appeared_pokemon->nombre_pokemon);
	appeared_pokemon->posicionX = atoi(argv[4]);
	appeared_pokemon->posicionY = atoi(argv[5]);

	if(!strcmp(tipo_modulo,"BROKER")){
		socket = conectarse_a(BROKER);
		enviar_mensaje_appeared(*appeared_pokemon, socket,4444,atoi(argv[6]));
	}else if(!strcmp(tipo_modulo,"TEAM")){
		socket = conectarse_a(TEAM);
		enviar_mensaje_appeared(*appeared_pokemon, socket,5002,0);
	}

	free(appeared_pokemon->nombre_pokemon);
	free(appeared_pokemon);
}

void gestionar_envio_get(char* argv[]){
	const char* tipo_modulo = argv[1];
	t_get_pokemon *get_pokemon = malloc (sizeof(t_get_pokemon));
	int socket;
	get_pokemon->nombre_pokemon = argv[3];
	get_pokemon->largo_nombre_pokemon = strlen(get_pokemon->nombre_pokemon);

	if(!strcmp(tipo_modulo,"BROKER")){
		log_info(mi_log,"0");
		socket = conectarse_a(BROKER);
			enviar_mensaje_get(*get_pokemon, socket,leer_puerto_broker(),0);
	}else if(!strcmp(tipo_modulo,"GAMECARD")){
		socket = conectarse_a(GAMECARD);
		enviar_mensaje_get(*get_pokemon, socket,leer_puerto_gamecard(),atoi(argv[3]));
	}

	free(get_pokemon->nombre_pokemon);
	free(get_pokemon);
}

void gestionar_envio_new(char* argv[]){
	const char* tipo_modulo = argv[1];
	t_new_pokemon *new_pokemon = malloc (sizeof(t_new_pokemon));
	int socket;
	new_pokemon->nombre_pokemon = argv[3];
	new_pokemon->largo_nombre_pokemon = strlen(new_pokemon->nombre_pokemon);
	new_pokemon->posicionX = atoi(argv[4]);
	new_pokemon->posicionY = atoi(argv[5]);
	new_pokemon->cantidad_pokemon = atoi(argv[6]);

	if(!strcmp(tipo_modulo,"BROKER")){
		socket = conectarse_a(BROKER);
		enviar_mensaje_new(*new_pokemon, socket,4444,0);
	}else if(!strcmp(tipo_modulo,"GAMECARD")){
		socket= conectarse_a(GAMECARD);
		enviar_mensaje_new(*new_pokemon, socket,5001,atoi(argv[6]));
	}

	free(new_pokemon->nombre_pokemon);
	free(new_pokemon);
}

void gestionar_envio_catch(char* argv[]){
	const char* tipo_modulo = argv[1];
	t_catch_pokemon *catch_pokemon = malloc (sizeof(t_catch_pokemon));
	int socket;
	catch_pokemon->nombre_pokemon = argv[3];
	catch_pokemon->largo_nombre_pokemon = strlen(catch_pokemon->nombre_pokemon);
	catch_pokemon->posicionX = atoi(argv[4]);
	catch_pokemon->posicionY = atoi(argv[5]);

	if(!strcmp(tipo_modulo,"BROKER")){
		socket = conectarse_a(BROKER);
		enviar_mensaje_catch(*catch_pokemon, socket,4444,0);
	}else if(!strcmp(tipo_modulo,"GAMECARD")){
		socket= conectarse_a(GAMECARD);
		enviar_mensaje_catch(*catch_pokemon, socket,5001,atoi(argv[6]));
	}

	free(catch_pokemon->nombre_pokemon);
	free(catch_pokemon);
}

void gestionar_envio_caught(char* argv[]){
	const char* tipo_modulo = argv[1];
	t_caught_pokemon *caught_pokemon = malloc (sizeof(caught_pokemon));
	int socket;
	caught_pokemon->atrapado = argv[4];
	socket = conectarse_a(BROKER);
	enviar_mensaje_caught(*caught_pokemon, socket,atoi(*argv[3]));
	free(caught_pokemon);
}
