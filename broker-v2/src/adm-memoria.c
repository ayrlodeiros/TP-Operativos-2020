/*#include "adm-memoria.h"

//PARTICIONES DINAMICAS

void inicializar_lista_particiones(){
	t_particion_dinamica* primera_part = malloc(sizeof(t_particion_dinamica));
	primera_part->inicio = 0;
	primera_part->fin = leer_tamano_memoria();
	primera_part->libre = true;
	lista_particiones_dinamicas = list_create();
	list_add(lista_particiones_dinamicas,primera_part);
}

int obtener_pos_particiones(int tamanio) {

	//pthread_mutex_lock(&mutex_memoria_principal);

	int posicion;
	int contador_compactacion = leer_frecuencia_compactacion();
	bool se_encontro_particion_libre = false;
	bool recien_se_compacto = false;

	while(se_encontro_particion_libre){
		posicion = buscar_particion_libre(tamanio);

		if( posicion >= 0 ){
			se_encontro_particion_libre = true;
			//nose si es mejor directamente retornar resultado aca
		}
		//el recien_se_compacto lo agrego para que en el caso que la frecuencia de compactacion sea cero, no se me quede en un loop infinito y nunca libere particiones
		else if (contador_compactacion == 0 && !recien_se_compacto)
		{
				compactacion();
				contador_compactacion = leer_frecuencia_compactacion();
				recien_se_compacto = true;
		}
			else {
				liberar_particion();
				contador_compactacion--;
				recien_se_compacto = false;
			}

	}
	return posicion;
	//pthread_mutex_unlock(&mutex_memoria_principal);

}

bool estaOcupado(void* elemento){
	return ((t_particion_dinamica*)elemento)->libre;
}

bool noEstaOcupado(void* elemento){
	return !((t_particion_dinamica*)elemento)->libre;
}

void borrarParticion(void* elemento){
	free((t_particion_dinamica*) elemento);
}

// Lo dejo work in progress, basicamente para la compactacion para eliminar los particiones libres pensaba
//bool libresPrimero(void* elemento,void* elementoDos){
//	if( ((t_particion_dinamica*)elemento)->libre && !((t_particion_dinamica*)elementoDos)->libre ){
//		return true;
//	}
//	if(){
//
//}


void compactacion(){

	t_list* particiones_ocupadas = obtener_particiones_ocupadas();
	//Hasta aca la lista de particiones, borre todas las estructuras que estaban libres y me quede con una nueva lista de particiones ocupadas
	//todo esta medio a lo bestia, despues ver si se puede mejorar

	t_list* lista_temporal = crear_list_temporal(particiones_ocupadas);;
	int prox_posicion = 0;
	for(int i = 0; list_size(lista_temporal);i++){

		t_struct_temporal* aux = list_get(lista_temporal,i);
		t_particion_dinamica* particion = aux->particion;
		list_add(lista_particiones_dinamicas,particion);
		particion->inicio = prox_posicion;
		particion->fin = prox_posicion + particion->tamanio_ocupado -1;
		prox_posicion = prox_posicion + particion->tamanio_ocupado;
		//todo acordarme de avisar a la estructura de msjs que se movieron los valores de la particion
		llenar_memoria_principal(particion->inicio,particion->tamanio_ocupado,aux->memoria);
		free(aux->memoria);
		free(aux);

	}

	//Creo la particion libre y la agrego al final de la memoria
	list_destroy(lista_temporal);
	t_particion_dinamica* particion_libre = crear_particion_dinamica_libre();
	particion_libre->inicio = prox_posicion;
	particion_libre->fin = leer_tamano_memoria() - 1;
	list_add(lista_particiones_dinamicas,particion_libre);

	}

void llenar_memoria_principal(int posicion, int tamanio, void* mensaje){
	memcpy(memoria_principal+posicion,mensaje,tamanio);
}

t_list* obtener_particiones_ocupadas(){
	t_list* particiones_ocupadas = list_filter(lista_particiones_dinamicas,estaOcupado);//filtrar lista de particiones llenas
	t_list* particiones_libres = list_filter(lista_particiones_dinamicas,noEstaOcupado);
	list_clean(lista_particiones_dinamicas);
	list_destroy_and_destroy_elements(particiones_libres,borrarParticion);
	return particiones_ocupadas;
}

t_list* crear_list_temporal(t_list* particiones){

	t_list* lista_temporal = list_create();

	for(int i = 0; list_size(particiones);i++){
		t_struct_temporal* aux = malloc(sizeof(t_struct_temporal));
		aux->particion = list_get(particiones,i);
		aux->memoria = malloc(aux->particion->tamanio_ocupado);
		memcpy(aux->memoria,memoria_principal+aux->particion->inicio,aux->particion->tamanio_ocupado);
		list_add(lista_temporal,aux);
	}
	return lista_temporal;
}

void liberar_particion(){
	int ubicacion_particion;
	switch(leer_algoritmo_reemplazo()){
		case FIFO:
				ubicacion_particion = algoritmo_reemplazo_fifo();
			break;
		case LRU:
				algoritmo_reemplazo_lru();
			break;
	}
	consolidar(ubicacion_particion);
}

//todo si hay tiempo, estas dos funciones son casi identicas, podrian abstraerse quedar mejor
int algoritmo_reemplazo_fifo(void){

	t_particion_dinamica* primera_particion = NULL;
		int pos_primera_particion;

		for (int i = 0; list_size(lista_particiones_dinamicas) > 0 ;i++){
			t_particion_dinamica* particion = list_get(lista_particiones_dinamicas,i);

			if(!esta_libre(particion)){

				if(primera_particion == NULL || particion->tiempo_ingreso < primera_particion->tiempo_ingreso){
					primera_particion = particion;
					pos_primera_particion = i;
				}
			}
		}

		borrar_msj_mp(primera_particion->inicio);
		primera_particion->libre = true;
		primera_particion->tamanio_ocupado = 0;

		//todo consolidar() tal vez conviene mejor ponerlo aca
		return pos_primera_particion;
}

int algoritmo_reemplazo_lru(void){

	t_particion_dinamica* part_menos_usada = NULL;
	int pos_part_menos_usada;

	for (int i = 0; list_size(lista_particiones_dinamicas) > 0 ;i++){
		t_particion_dinamica* particion = list_get(lista_particiones_dinamicas,i);

		if(!esta_libre(particion)){

			if(part_menos_usada == NULL || particion->ult_vez_usado < part_menos_usada->ult_vez_usado){
				part_menos_usada = particion;
				pos_part_menos_usada = i;
			}
		}
	}

	borrar_msj_mp(part_menos_usada->inicio);
	part_menos_usada->libre = true;
	part_menos_usada->tamanio_ocupado = 0;
	//todo part_menos_usada->orden_ingreso;
	//todo consolidar() tal vez conviene mejor ponerlo aca
	return pos_part_menos_usada;
}

// Supongo que funciona pero seguro se puede mejorar
void consolidar(int pos_particion){

	t_particion_dinamica* liberada = list_get(lista_particiones_dinamicas,pos_particion);
	t_particion_dinamica* aux = liberada;
	int pos = pos_particion;

	if(particion_libre_a_la_izquierda(pos_particion)){
		t_particion_dinamica* izquierda = list_get(lista_particiones_dinamicas,pos_particion-1);
		izquierda->fin = liberada->fin;
		list_remove(lista_particiones_dinamicas,pos_particion);
		free(liberada);
		aux = izquierda;
		pos = pos_particion -1;
	}
	if (particion_libre_a_la_derecha(pos)){

		t_particion_dinamica* derecha = list_get(lista_particiones_dinamicas,pos+1);
		aux->inicio = derecha->inicio;
		list_remove(lista_particiones_dinamicas,pos+1);
		free(derecha);
	}
}

bool particion_libre_a_la_izquierda(int posicion){
	return posicion - 1 >= 0 && esta_libre(list_get(lista_particiones_dinamicas,posicion-1));

}

bool particion_libre_a_la_derecha(int posicion){
	return posicion + 1 <= leer_tamano_memoria()-1 && esta_libre(list_get(lista_particiones_dinamicas,posicion+1));
}


int buscar_particion_libre(int tamanio){

	switch(leer_algoritmo_particion_libre()){
		case FF:
			return algoritmo_first_fit(tamanio);
			break;
		case BF:
			return algoritmo_best_fit(tamanio);
			break;
		}
	return -1;
}

int algoritmo_best_fit(int tamanio){
	t_particion_dinamica* mejor_particion;
	int dif_mejor_part = -1;
	int posicion_mejor_part;

	for(int i = 0; list_size(lista_particiones_dinamicas) > i ; i++ ){
		t_particion_dinamica* particion_actual = list_get(lista_particiones_dinamicas,i);


		if(esta_libre(particion_actual)){
			int diferencia = diferencia_tamanio_particion(particion_actual,tamanio);

			if(diferencia >= 0){

				if(diferencia < dif_mejor_part || dif_mejor_part < 0){
					mejor_particion = particion_actual;
					dif_mejor_part = diferencia;
					posicion_mejor_part = i;
				}
			}
		}
	}

	if(dif_mejor_part < 0){

		return -1;
	}
	else if(dif_mejor_part >= leer_tamano_minimo_particion())
		{
			return llenar_y_realizar_nueva_particion(mejor_particion,tamanio,posicion_mejor_part);
		}
		else
		{
			return llenar_particion(mejor_particion,tamanio);
		}

}

int algoritmo_first_fit(int tamanio){

	t_particion_dinamica* particion;

	for(int i = 0;list_size(lista_particiones_dinamicas) > i; i++){
		particion = list_get(lista_particiones_dinamicas,i);

		if(esta_libre(particion)){
			// Esto es porque hay un minimo en el tamanio de particiones
			int diferencia = diferencia_tamanio_particion(particion,tamanio);

			if(diferencia >= 0){

				if(diferencia >= leer_tamano_minimo_particion())
				{
						return llenar_y_realizar_nueva_particion(particion,tamanio,i);
				}
				else
				{
						return llenar_particion(particion,tamanio);
				}
			}
		}
	}
	// No se encontro espacio libre para el msj
	return -1;
}

t_particion_dinamica* crear_particion_dinamica_libre(){
	t_particion_dinamica* particion = malloc(sizeof(t_particion_dinamica));
	particion->libre = true;
	return particion;
}

int llenar_y_realizar_nueva_particion(t_particion_dinamica* particion,int tamanio,int posicion_en_lista){
	//todo no ovlidar que cuando elimino la particion se tiene que liberar este espacio de memoria
	t_particion_dinamica* nueva_particion = crear_particion_dinamica_libre();
	nueva_particion->fin = particion->fin;
	nueva_particion->inicio = nueva_particion->fin - tamanio + 1;


	//todo aunque tecnicamente no esta ocupado todavia no se me ocurre otro momento mejor para llenar este dato
	particion->tamanio_ocupado = tamanio;
	particion->fin = particion->inicio + tamanio - 1;
	particion->libre = false;
	particion->tiempo_ingreso = timestamp();
	particion->ult_vez_usado = timestamp();

	list_add_in_index(lista_particiones_dinamicas,posicion_en_lista+1,nueva_particion);

	return particion->inicio;

}
// A diferencia del de arriba esto es cuando el msj tiene el mismo tamanio que la particion o un poco menos
int llenar_particion(t_particion_dinamica* particion, int tamanio){
	particion->tamanio_ocupado = tamanio;
	particion->libre = false;
	particion->tiempo_ingreso = timestamp();
	particion->ult_vez_usado = timestamp();
	return particion->inicio;
}

bool esta_libre(t_particion_dinamica* particion){
	return particion->libre;
}

int diferencia_tamanio_particion(t_particion_dinamica* particion,int tamanio_msj){
	return tamanio_particion(particion) - tamanio_msj;
}

int tamanio_particion(t_particion_dinamica* particion){
	return particion->fin - particion->inicio +1;
}


// BUDDY SYSTEM

void inicializar_lista_bs(){
	int tamanio_memoria = leer_tamano_memoria();

	lista_particiones_bs = list_create();
	t_particion_bs* primera_part = malloc(sizeof(t_particion_bs));
	primera_part->inicio = 0;
	primera_part->fin = tamanio_memoria; // Ver si usar el -1
	primera_part->potencia_de_dos=obtener_potencia_de_dos_mas_cercana(tamanio_memoria);
	primera_part->tiempo_ingreso = timestamp();
	primera_part->ult_vez_usado = timestamp();
	primera_part->libre = true;

}

int obtener_posicion_bs(int tamanio) {

	int potencia_de_dos_mas_cercana = obtener_potencia_de_dos_mas_cercana(tamanio);

	pthread_mutex_lock(&mutex_memoria_principal);

	//OBTIENE LA POSICION DE LA PARTICION QUE MAS SE ACERCA A LA POTENCIA DE DOS DESEADA, DEVUELVE -1 SI NO ENCONTRO NIGUNA
	int posicion_de_particion_en_lista = obtener_posicion_particion_mas_cercana(potencia_de_dos_mas_cercana);

	//REALIZAR LIBERACION Y CONSOLIDACION, HASTA OBTENER posicion DISTINTA DE -1
	while(posicion_de_particion_en_lista == -1) {
		liberar_y_consolidar();

		posicion_de_particion_en_lista = obtener_posicion_particion_mas_cercana(potencia_de_dos_mas_cercana);
	}


	t_particion_bs* posible_particion = list_get(lista_particiones_bs, posicion_de_particion_en_lista);

	//SI LA POTENCIA DE DOS DE LA PARTICION ENCONTRADA NO ES IGUAL A LA DESEADA SE PARTICIONA A LA PARTICION
	if(posible_particion->potencia_de_dos != potencia_de_dos_mas_cercana) {
		// DIVIDIR EN DOS HASTA LLEGAR A POTENCIA DE DOS MAS CERCANA
		posible_particion = particionar_y_obtener_particion(posicion_de_particion_en_lista, potencia_de_dos_mas_cercana);
	}

	// OCUPAR LA PARTICION
	posible_particion->libre = false;
	posible_particion->tiempo_ingreso = timestamp();
	posible_particion->ult_vez_usado = timestamp();

	pthread_mutex_unlock(&mutex_memoria_principal);

	return posible_particion->inicio;
}

int obtener_potencia_de_dos_mas_cercana(int valor) {
	double aux = valor;
	int cont = 0;
	if(valor < 2) {
		return 1;
	} else {
		while(aux > 1) {
			aux /= 2;
			cont ++;
		}
		return cont;
	}
}

int obtener_posicion_particion_mas_cercana(int potencia_de_dos) {
	int posicion = -1;

	for(int i = 0; i < list_size(lista_particiones_bs); i++) {
		t_particion_bs* posible_particion = list_get(lista_particiones_bs, i);
		if(posible_particion->libre) {
			if(posible_particion->potencia_de_dos == potencia_de_dos) {
				return i;
			}
			if(posible_particion->potencia_de_dos > potencia_de_dos) {
				posicion = i;
			}
		}
	}

	return posicion;
}

void liberar_y_consolidar() {
	int posicion_particion_liberada = liberar_una_particion();

	int posicion_consolidacion = evaluar_consolidacion(posicion_particion_liberada);
	//Hasta que no devuelva -1 quiere decir que se puede seguir intentando consolidar la misma particion
	while(posicion_consolidacion != -1) {
		posicion_consolidacion = evaluar_consolidacion(posicion_consolidacion);
	}
}

int liberar_una_particion() {
	int posicion_particion_liberada;

	switch(leer_algoritmo_reemplazo()){
		case FIFO:
			posicion_particion_liberada = obtener_posicion_de_particion_liberada_fifo();
			break;
		case LRU:
			posicion_particion_liberada = obtener_posicion_de_particion_liberada_lru();
			break;
	}

	return posicion_particion_liberada;
}

int obtener_posicion_de_particion_liberada_fifo() {
	t_particion_bs* particion_objetivo = NULL;
	int posicion;

	for(int i = 0; i<list_size(lista_particiones_bs); i++) {
		t_particion_bs* particion_aux = list_get(lista_particiones_bs, i);
		if(!(particion_aux->libre) && (particion_objetivo == NULL || particion_aux->tiempo_ingreso < particion_objetivo->tiempo_ingreso)) {
			particion_objetivo = particion_aux;
			posicion = i;
		}
	}

	borrar_msj_mp(particion_objetivo->inicio);
	particion_objetivo->libre = true;

	return particion_objetivo;
}

int obtener_posicion_de_particion_liberada_lru() {
	t_particion_bs* particion_objetivo = NULL;
	int posicion;

	for(int i = 0; i<list_size(lista_particiones_bs); i++) {
		t_particion_bs* particion_aux = list_get(lista_particiones_bs, i);
		if(!(particion_aux->libre) && (particion_objetivo == NULL || particion_aux->ult_vez_usado < particion_objetivo->ult_vez_usado)) {
			particion_objetivo = particion_aux;
			posicion = i;
		}
	}

	borrar_msj_mp(particion_objetivo->inicio);
	particion_objetivo->libre = true;

	return posicion;
}

// EVALUA SI SE PUEDE CONSOLIDAR, Y CONSOLIDA DE SER POSIBLE, SI CONSOLIDO DEVUELVE LA POSICION EN LA QUE QUEDO LA PARTICION CONSOLIDADA, SI NO -1
int evaluar_consolidacion(int posicion_buddy_1) {
	int posicion_buddy_2;
	//CONSIGO LAS POSICIONES DE LOS DOS BUDDIES
	if(posicion_buddy_1%2 == 0) {
		posicion_buddy_2 = posicion_buddy_1 + 1;
	} else {
		posicion_buddy_2 = posicion_buddy_1 - 1;
	}

	t_particion_bs* buddy_2 = list_get(lista_particiones_bs, posicion_buddy_2);

	if(buddy_2->libre) {
		int posicion_mas_chica;
		if(posicion_buddy_1 > posicion_buddy_2) {
			posicion_mas_chica = posicion_buddy_2;
			consolidar_buddies(posicion_buddy_1, buddy_2);
		} else {
			posicion_mas_chica = posicion_buddy_1;
			consolidar_buddies(posicion_buddy_2, list_get(lista_particiones_bs, posicion_buddy_1));
		}

		return posicion_mas_chica;
	}

	return -1;
}

void consolidar_buddies(int posicion_buddy_a_eliminar, t_particion_bs* buddy_a_mantener) {
	t_particion_bs* buddy_eliminado = list_remove(lista_particiones_bs, posicion_buddy_a_eliminar);


	buddy_a_mantener->potencia_de_dos++;
	buddy_a_mantener->fin = buddy_eliminado->fin;


	free(buddy_eliminado);
}

t_particion_bs* particionar_y_obtener_particion(int posicion_a_particionar, int potencia_de_dos_deseada) {
	t_particion_bs* particion_a_particionar = list_get(lista_particiones_bs, posicion_a_particionar);

	// TODO DESCOMENTAR Y VER PORQUE NO ME RECONOCE EL pow (#include <math.h> EN constructor.h)
	int tamanio_actual = pow(2, particion_a_particionar->potencia_de_dos);
	int tamanio_deseado = pow(2, potencia_de_dos_deseada);

	t_list* lista_auxiliar = list_create();
	while(tamanio_actual != tamanio_deseado) {
		tamanio_actual /= 2;
		int potencia_nueva = obtener_potencia_de_dos_mas_cercana(tamanio_actual);

		t_particion_bs* nueva_particion = malloc(sizeof(t_particion_bs));
		nueva_particion->inicio = particion_a_particionar->inicio + tamanio_actual;
		nueva_particion->fin = nueva_particion->inicio + tamanio_actual; // TODO si se usa el -1 cuando se crea, habria que usarlo aca tambien
		nueva_particion->potencia_de_dos = potencia_nueva;
		nueva_particion->libre = true;
		list_add(lista_auxiliar, nueva_particion);

		particion_a_particionar->fin = nueva_particion->inicio; // TODO si se usa el -1 cuando se crea, habria que usarlo aca tambien
	}

	particion_a_particionar->potencia_de_dos = potencia_de_dos_deseada;

	// AGREGO LAS NUEVAS PARTICIONES A LA LISTA DE PARTICIONES, DETRAS DE LA PARTICION QUE CREE
	posicion_a_particionar++;
	for(int i = 0; i < list_size(lista_auxiliar); i++) {
		list_add_in_index(lista_particiones_bs, posicion_a_particionar+i, list_get(lista_auxiliar, i));
	}


	list_destroy(lista_auxiliar);
	return particion_a_particionar;
}

int potencia(int base, int exponente) {
	int resultado = 1;
	for(int i = 0; i<exponente; i++) {
		resultado = resultado * base;
	}
	return resultado;
}


uint64_t timestamp(void) {

	struct timeval valor;
	gettimeofday(&valor, NULL);
	unsigned long long result = (((unsigned long long )valor.tv_sec) * 1000 + ((unsigned long) valor.tv_usec));
	uint64_t tiempo = result;

	return tiempo;

}

//todo tengo la duda, habria que al inicializar la memoria principal setear los valores a 0 o algo para identificarlo, agregar semaforos
void borrar_msj_mp(int posicion){
	//memset(memoria_principal+posicion,0,tamanio);
	//de alguna forma hay que avisar a la estructura de mensajes que se elimino este msj

	pthread_mutex_lock(&mutex_lista_msjs);
	for(int i=0; i< list_size(lista_global_msjs); i++) {
		t_mensaje* mensaje = list_get(lista_global_msjs, i);
		if(mensaje->pos_en_memoria->pos == posicion) {
			list_remove_and_destroy_element(lista_global_msjs, i, destruir_t_mensaje);
			break;
		}
	}
	pthread_mutex_unlock(&mutex_lista_msjs);
}

void destruir_t_mensaje(t_mensaje* mensaje) {
	free(mensaje->pos_en_memoria);
	list_destroy(mensaje->suscriptores_conf);
	list_destroy(mensaje->suscriptores_env);
	free(mensaje);
}*/
