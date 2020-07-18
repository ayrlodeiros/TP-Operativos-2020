#include "constructor.h"

//Se deberia ejecutar una sola vez en el team
void iniciar_variables_globales() {
	armar_entrenadores();
	armar_objetivo_global();
<<<<<<< HEAD
	pokemons_sin_entrenador = queue_create();
	entrenadores_ready = list_create();
	pthread_mutex_lock(&lock_de_planificacion);
=======
	armar_pokemons_para_recibir();
	pokemons_sin_entrenador = queue_create();
	pokemons_en_espera = list_create();
	entrenadores_ready = list_create();
	lista_ids_localized = list_create();
	lista_ids_caught = list_create();
	intercambios = list_create();

	pthread_mutex_init(&lock_de_planificacion, NULL);
	pthread_mutex_init(&lock_de_entrenador_disponible, NULL);
	pthread_mutex_init(&lock_reintento_broker, NULL);
	pthread_mutex_init(&mutex_entrenadores, NULL);
	pthread_mutex_init(&mutex_pokemon_para_planificar, NULL);
	pthread_mutex_init(&mutex_entrenadores_ready, NULL);
	pthread_mutex_init(&mutex_lista_ids_localized, NULL);
	pthread_mutex_init(&mutex_lista_ids_caught, NULL);
	pthread_mutex_init(&mutex_funciona_broker, NULL);
	pthread_mutex_init(&mutex_objetivo_global, NULL);
	pthread_mutex_init(&mutex_pokemons_recibidos, NULL);
	pthread_mutex_init(&mutex_cantidad_de_deadlocks, NULL);
	pthread_mutex_init(&mutex_pokemons_sin_entrenador, NULL);
	pthread_mutex_init(&mutex_pokemons_en_espera, NULL);

	pthread_mutex_lock(&lock_de_planificacion);
	pthread_mutex_lock(&lock_de_entrenador_disponible);
	pthread_mutex_lock(&lock_reintento_broker);

	conexion_appeared = -1;
	conexion_localized = -1;
	conexion_caught = -1;
	funciona_broker = 0;
	cantidad_de_deadlocks = 0;
>>>>>>> develop
}

//Se deberia ejecutar una sola vez, en el metodo inciar_variables_globales
void armar_entrenadores() {
	entrenadores = list_create();

	t_list* posiciones = leer_posiciones_entrenadores();
	t_list* pokemons = leer_pokemon_entrenadores();
	t_list* objetivos = leer_objetivos_entrenadores();

	for(int i=0; i< list_size(posiciones);i++){

		char* posicion_entrenador = list_get(posiciones, i);
		char* pokemons_entrenador;

		if(i<list_size(pokemons)){
			pokemons_entrenador = list_get(pokemons, i);
		}else{
			pokemons_entrenador = "nulo";
		}
		char* objetivos_entrenador =  list_get(objetivos, i);

		list_add(entrenadores,armar_entrenador(posicion_entrenador,pokemons_entrenador,objetivos_entrenador,i));
	}

	list_destroy(posiciones);
	list_destroy(pokemons);
	list_destroy(objetivos);

}

//Se deberia usar solo en armar_entrenadores
entrenador* armar_entrenador(char* posicion, char* pokemons, char* objetivos, int id){
	entrenador* un_entrenador = malloc(sizeof(entrenador));

	if(string_equals_ignore_case(pokemons,"nulo")){
		un_entrenador->pokemons_adquiridos = list_create();
	}else{
		char** pokemons_adquiridos = string_split(pokemons,"|");
		t_list* lista_pokemons_adquiridos = crear_t_list(pokemons_adquiridos);
		free(pokemons_adquiridos);
		un_entrenador->pokemons_adquiridos = lista_pokemons_adquiridos;
	}
	char** pokemons_objetivo = string_split(objetivos,"|");

	t_list* lista_pokemons_objetivo = crear_t_list(pokemons_objetivo);

	free(pokemons_objetivo);


	un_entrenador->id = id;
	un_entrenador->posicion = armar_posicion(posicion);
	un_entrenador->pokemons_objetivo = lista_pokemons_objetivo;
	un_entrenador->pokemons_sobrantes = list_create();
	un_entrenador->cant_maxima_pokemons = list_size(lista_pokemons_objetivo);
	un_entrenador->cpu_usado = 0;
	un_entrenador->cpu_disponible = 0;
<<<<<<< HEAD
	un_entrenador->cpu_estimado_anterior = estimacion_inicial;
	un_entrenador->acciones = queue_create();
=======
	un_entrenador->cpu_rr_anterior = 0;
	un_entrenador->cpu_estimado_anterior = leer_estimacion_inicial();
	un_entrenador->cpu_estimado_restante = un_entrenador->cpu_estimado_anterior;
	un_entrenador->cpu_sjf_anterior = 1;
	un_entrenador->acciones = list_create();

	actualizar_objetivo_y_sobrante_del_entrenador_con_adquiridos(un_entrenador);

	if(el_entrenador_cumplio_su_objetivo(un_entrenador)){
		log_info(nuestro_log,"El entrenador %d cumplio su objetivo y queda en estado EXIT", un_entrenador->id);
		cambiar_estado_entrenador(un_entrenador,EXIT);
	}else if (el_entrenador_no_puede_capturar_mas_pokemons(un_entrenador)) {
		log_info(nuestro_log,"El entrenador %d se bloquea quedando en estado DEADLOCK", un_entrenador->id);
		cambiar_estado_entrenador(un_entrenador,BLOCK_DEADLOCK);
	}
	else{
		cambiar_estado_entrenador(un_entrenador,NEW);
	}
>>>>>>> develop

	return un_entrenador;
}

//ESTO SOLO SE LLAMA AL ARMAR LOS ENTRENADORES
void actualizar_objetivo_y_sobrante_del_entrenador_con_adquiridos(entrenador* entrenador) {
	for(int i = 0; i < list_size(entrenador->pokemons_adquiridos); i++) {
		actualizar_objetivo_y_sobrante_del_entrenador(entrenador, list_get(entrenador->pokemons_adquiridos, i));
	}
}

void actualizar_objetivo_y_sobrante_del_entrenador(entrenador* entrenador, char* pokemon_adquirido) {
	int posicion_del_pokemon_en_lista = devolver_posicion_en_la_lista_del_pokemon(entrenador->pokemons_objetivo, pokemon_adquirido);
	if(posicion_del_pokemon_en_lista == -1) {
		list_add(entrenador->pokemons_sobrantes, pokemon_adquirido);
	} else {
		list_remove(entrenador->pokemons_objetivo, posicion_del_pokemon_en_lista);
	}
}

int devolver_posicion_en_la_lista_del_pokemon(t_list* lista_pokemons, char* pokemon_a_buscar){
	for(int i = 0; i< list_size(lista_pokemons);i++){
		if(string_equals_ignore_case(list_get(lista_pokemons, i), pokemon_a_buscar)){
			return i;
		}
	}
	return -1;
}

//Se deberia ejecutar una sola vez, en el metodo inciar_variables_globales
void armar_objetivo_global() {
	objetivo_global = dictionary_create();

	//Recorro los objetivos de los entrenadores para agregarlos al objetivo global
	for(int i = 0; i<list_size(entrenadores); i++) {
		entrenador* entrenador_aux = list_get(entrenadores, i);
		for(int j = 0; j<list_size(entrenador_aux->pokemons_objetivo); j++) {
			agregar_objetivo_a_objetivo_global(list_get(entrenador_aux->pokemons_objetivo, j));
		}
		for(int k = 0; k<list_size(entrenador_aux->pokemons_sobrantes); k++) {
			restar_adquirido_a_objetivo_global(list_get(entrenador_aux->pokemons_sobrantes, k));
		}
	}
}

//Se deberia usar solo en armar_objetivo_global y cuando no se pudo capturar un pokemon
void agregar_objetivo_a_objetivo_global(char* pokemon_objetivo) {
	pthread_mutex_lock(&mutex_objetivo_global);
	//Si el pokemon ya existia en el objetivo global, obtengo el valor que tenia y le sumo uno
	if(dictionary_has_key(objetivo_global, pokemon_objetivo)) {
		dictionary_put(objetivo_global, pokemon_objetivo, (void*) (dictionary_get(objetivo_global, pokemon_objetivo) + 1));
	}
	//Si el pokemon no existia lo agrego al diccionario con un valor de 1
	else {
		int valor_inicial = 1;
		dictionary_put(objetivo_global, pokemon_objetivo, valor_inicial);
	}
	pthread_mutex_unlock(&mutex_objetivo_global);
}

//Resto el pokemon atrapado del objetivo global y cuando se captura un pokemon
void restar_adquirido_a_objetivo_global(char* pokemon_adquirido) {
<<<<<<< HEAD
	dictionary_put(objetivo_global, pokemon_adquirido, dictionary_get(objetivo_global, pokemon_adquirido)-1);
=======
	pthread_mutex_lock(&mutex_objetivo_global);
	dictionary_put(objetivo_global, pokemon_adquirido, (void*) (dictionary_get(objetivo_global, pokemon_adquirido) - 1));
	pthread_mutex_unlock(&mutex_objetivo_global);
>>>>>>> develop
}

posicion* armar_posicion(char* posicion_a_armar) {
	posicion* pos = malloc(sizeof(posicion));

	char** posiciones = string_split(posicion_a_armar,"|");

	pos->posicion_x = atoi(posiciones[0]);
	pos->posicion_y = atoi(posiciones[1]);

	free(posiciones[0]);
	free(posiciones[1]);
	free(posiciones);

	return pos;
}

accion* armar_accion(void(*funcion)(void*), int cpu_requerido){
	accion* nueva_accion = malloc(sizeof(accion));

	nueva_accion->funcion = funcion;
	nueva_accion->cpu_requerido = cpu_requerido;

	return nueva_accion;
}
<<<<<<< HEAD
=======

void armar_pokemons_para_recibir() {
	pokemons_recibidos = list_create();

	dictionary_iterator(objetivo_global, evaluar_pokemon_para_recibir);

}

void evaluar_pokemon_para_recibir(char* key, void* value) {
	if(value > 0) {
		recepcion_pokemon* rec_pok = malloc(sizeof(recepcion_pokemon));
		rec_pok->pokemon = key;
		rec_pok->fue_recibido_un_msj = 0;

		list_add(pokemons_recibidos, rec_pok);
	}
}
>>>>>>> develop
