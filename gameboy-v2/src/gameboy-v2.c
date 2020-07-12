#include "gameboy-v2.h"

int main(int argc, char *argv[])
{
	iniciar_gameBoy();

	if(argc >= 4){
		char* primer_argumento = argv[1];
		char* tipo_mensaje = argv[2];
		if(!strcmp(primer_argumento,"SUSCRIPTOR")){
			if(argc == 4) {
				gestionar_suscriptor(argv);
			} else {
				log_error(mi_log, "Cantidad incorrecta de argumentos para SUSCRIPCION");
			}
		} else if(!strcmp(tipo_mensaje,"APPEARED_POKEMON")){
			gestionar_envio_appeared(argv, argc);
		} else if(!strcmp(tipo_mensaje,"GET_POKEMON")){
			gestionar_envio_get(argv, argc);
		} else if(!strcmp(tipo_mensaje,"NEW_POKEMON")){
			gestionar_envio_new(argv, argc);
		} else if(!strcmp(tipo_mensaje,"CATCH_POKEMON")){
			gestionar_envio_catch(argv, argc);
		} else if(!strcmp(tipo_mensaje,"CAUGHT_POKEMON")){
			if(argc == 5) {
				gestionar_envio_caught(argv);
			} else {
				log_error(mi_log, "Cantidad incorrecta de argumentos para CAUGHT");
			}
		} else {
			log_error(mi_log, "MENSAJE INCORRECTO");
		}
	} else {
		log_error(mi_log, "MENSAJE INCORRECTO, NO CUMPLE CON LA CANTIDAD MINIMA DE ARGUMENTOS");
	}

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

void gestionar_envio_appeared(char* argv[], int argc){
	t_appeared_pokemon *appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
	int socket;
	char* tipo_modulo = argv[1];

	appeared_pokemon->nombre_pokemon = argv[3];
	appeared_pokemon->largo_nombre_pokemon = strlen(appeared_pokemon->nombre_pokemon);
	appeared_pokemon->posicionX = atoi(argv[4]);
	appeared_pokemon->posicionY = atoi(argv[5]);

	if(!strcmp(tipo_modulo,"BROKER")){
		if(argc == 7) {
			socket = conectarse_a(BROKER);
			enviar_mensaje_appeared_broker(*appeared_pokemon, socket,atoi(argv[6]));
		} else {
			log_error(mi_log, "Cantidad incorrecta de argumentos para APPEARED en BROKER");
		}
	} else if(!strcmp(tipo_modulo,"TEAM")){
		if(argc == 6) {
			socket = conectarse_a(TEAM);
			enviar_mensaje_appeared_team(*appeared_pokemon, socket);
		} else {
			log_error(mi_log, "Cantidad incorrecta de argumentos para APPEARED en TEAM");
		}
	} else {
		log_error(mi_log, "Modulo incorrecto para APPEARED");
	}

	//ESTE FREE PROVOCA EL SEG F
	//free(appeared_pokemon->nombre_pokemon);
	free(appeared_pokemon);
}

void gestionar_envio_get(char* argv[], int argc){
	const char* tipo_modulo = argv[1];
	t_get_pokemon *get_pokemon = malloc (sizeof(t_get_pokemon));
	int socket;
	get_pokemon->nombre_pokemon = argv[3];
	get_pokemon->largo_nombre_pokemon = strlen(get_pokemon->nombre_pokemon);

	if(!strcmp(tipo_modulo,"BROKER")){
		if(argc == 4) {
			socket = conectarse_a(BROKER);
			enviar_mensaje_get_broker(*get_pokemon,socket);
		} else {
			log_error(mi_log, "Cantidad incorrecta de argumentos para GET en BROKER");
		}
	} else if(!strcmp(tipo_modulo,"GAMECARD")){
		if(argc == 5) {
			socket = conectarse_a(GAMECARD);
			enviar_mensaje_get_gamecard(*get_pokemon, socket,atoi(argv[3]));
		} else {
			log_error(mi_log, "Cantidad incorrecta de argumentos para GET en GAMECARD");
		}
	} else {
		log_error(mi_log, "Modulo incorrecto para GET");
	}

	free(get_pokemon);
}

void gestionar_envio_new(char* argv[], int argc){
	const char* tipo_modulo = argv[1];
	t_new_pokemon *new_pokemon = malloc (sizeof(t_new_pokemon));
	int socket;
	//TODO VER DE ABSTRAER EN FUNCION PARA LLAMARLO RECIEN CUANDO SE HIZO LA VALIDACION
	new_pokemon->nombre_pokemon = argv[3];
	new_pokemon->largo_nombre_pokemon = strlen(new_pokemon->nombre_pokemon);
	new_pokemon->posicionX = atoi(argv[4]);
	new_pokemon->posicionY = atoi(argv[5]);
	new_pokemon->cantidad_pokemon = atoi(argv[6]);

	if(!strcmp(tipo_modulo,"BROKER")){
		if(argc == 7) {

			socket = conectarse_a(BROKER);
			enviar_mensaje_new_broker(*new_pokemon, socket);
		} else {
			log_error(mi_log, "Cantidad incorrecta de argumentos para NEW en BROKER");
		}
	} else if(!strcmp(tipo_modulo,"GAMECARD")){
		if(argc == 8) {

			socket= conectarse_a(GAMECARD);
			enviar_mensaje_new_gamecard(*new_pokemon, socket,atoi(argv[6]));
		} else {
			log_error(mi_log, "Cantidad incorrecta de argumentos para NEW en GAMECARD");
		}
	} else {
		log_error(mi_log, "Modulo incorrecto para NEW");
	}

	free(new_pokemon);
}



void gestionar_envio_catch(char* argv[], int argc){
	const char* tipo_modulo = argv[1];
	t_catch_pokemon *catch_pokemon = malloc (sizeof(t_catch_pokemon));
	int socket;
	catch_pokemon->nombre_pokemon = argv[3];
	catch_pokemon->largo_nombre_pokemon = strlen(catch_pokemon->nombre_pokemon);
	catch_pokemon->posicionX = atoi(argv[4]);
	catch_pokemon->posicionY = atoi(argv[5]);

	if(!strcmp(tipo_modulo,"BROKER")){
		if(argc == 6) {
			socket = conectarse_a(BROKER);
			enviar_mensaje_catch_broker(*catch_pokemon, socket);
		} else {
			log_error(mi_log, "Cantidad incorrecta de argumentos para CATCH en BROKER");
		}
	} else if(!strcmp(tipo_modulo,"GAMECARD")){
		if(argc == 7) {
			socket= conectarse_a(GAMECARD);
			enviar_mensaje_catch_gamecard(*catch_pokemon, socket,atoi(argv[6]));
		} else {
			log_error(mi_log, "Cantidad incorrecta de argumentos para CATCH en GAMECARD");
		}
	} else {
		log_error(mi_log, "Modulo incorrecto para CATCH");
	}

	free(catch_pokemon);
}

void gestionar_envio_caught(char* argv[]){
	t_caught_pokemon *caught_pokemon = malloc (sizeof(caught_pokemon));
	int socket;
	if(!strcmp(argv[4],"OK")){
		caught_pokemon->atrapado = 1;
	}else if(!strcmp(argv[4],"FAIL")){
		caught_pokemon->atrapado = 0;
	}
	socket = conectarse_a(BROKER);
	enviar_mensaje_caught(*caught_pokemon, socket,atoi(argv[3]));

	free(caught_pokemon);
}


void gestionar_suscriptor(char* argv[]){
	int socket;
	socket = conectarse_a(BROKER);
	int cola = cola_mensajes(argv[2]);

	log_info(mi_log,string_from_format("cola: %d",cola));
	suscribirse_a_cola(cola, atoi(argv[3]),socket);
}

int cola_mensajes(char* nombre_cola){
	if(!strcmp(nombre_cola,"GET_POKEMON")){
		return GET;
	}else if(!strcmp(nombre_cola,"APPEARED_POKEMON")){
		return APPEARED;
	}else if(!strcmp(nombre_cola,"NEW_POKEMON")){
		return NEW;
	}else if(!strcmp(nombre_cola,"CAUGHT_POKEMON")){
		return CAUGHT;
	}else if(!strcmp(nombre_cola,"CATCH_POKEMON")){
		return CATCH;
	}else if(!strcmp(nombre_cola,"LOCALIZED_POKEMON")){
		return LOCALIZED;
	}
	return -1;
}
