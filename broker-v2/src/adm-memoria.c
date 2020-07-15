#include "adm-memoria.h"

//PARTICIONES DINAMICAS

void inicializar_lista_particiones(){
	t_particion* primera_part = malloc(sizeof(t_particion));
	primera_part->inicio = 0;
	primera_part->fin = leer_tamano_memoria();
	primera_part->libre = true;
	lista_particiones = list_create();
	list_add(lista_particiones,primera_part);
}

void inicializar_contador_compactacion(){
	contador_compactacion = leer_frecuencia_compactacion();
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
		//el recien_Se_compacto lo agrego para que en el caso que la frecuencia de compactacion sea cero, no se me quede en un loop infinito y nunca libere particiones
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

void compactacion(){
	//todo definir compactacion

	// Basicamente resetea el valor del contador para que arranque de nuevo
	inicializar_contador_compactacion();
}

void liberar_particion(){

	switch(leer_algoritmo_reemplazo()){
		case FIFO:
			break;
		case LRU:
			break;
	}
	consolidar();
}

void consolidar();


int buscar_particion_libre(int tamanio){
	switch(leer_algoritmo_particion_libre()){
		case FF:
			return algoritmo_first_fit(tamanio);
			break;
		case BF:
			return algoritmo_best_fit(tamanio);
			break;
		}
}

int algoritmo_best_fit(int tamanio){
	t_particion* mejor_particion;
	for(int i = 0; list_size(lista_particiones) > i ; i++ ){
		t_particion* particion_actual = list_get(lista_particiones,i);

		if(mejor_particion != NULL){

		}
		else
			mejor_particion = list_get(lista_particiones,i);
	}
	return -1;
}

int algoritmo_first_fit(int tamanio){

	t_particion* particion;

	for(int i = 0;list_size(lista_particiones) > i; i++){
		particion = list_get(lista_particiones,i);

		if(esta_libre(particion)){
			/* Esto es porque hay un minimo en el tamanio de particiones */
			int diferencia = tamanio_particion(particion) - tamanio;

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
	/* No se encontro espacio libre para el msj */
	return -1;
}


int llenar_y_realizar_nueva_particion(t_particion* particion,int tamanio,int posicion_en_lista){
	t_particion* nueva_particion = malloc(sizeof(t_particion));
	nueva_particion->fin = particion->fin;
	nueva_particion->inicio = nueva_particion->fin - tamanio + 1;

	//todo aunque tecnicamente no esta ocupado todavia no se me ocurre otro momento mejor para llenar este dato
	particion->tamanio_ocupado = tamanio;
	particion->fin = particion->inicio + tamanio - 1;
	particion->libre = false;

	list_add_in_index(lista_particiones,posicion_en_lista+1,nueva_particion);

	return particion->inicio;

}
/* A diferencia del de arriba esto es cuando el msj tiene el mismo tamanio que la particion o un poco menos*/
int llenar_particion(t_particion* particion, int tamanio){
	particion->tamanio_ocupado = tamanio;
	particion->libre = false;
	return particion->inicio;
}

bool esta_libre(t_particion* particion){
	return particion->libre;
}

int tamanio_particion(t_particion* particion){
	return particion->fin - particion->inicio +1;
}



/* BUDDY SYSTEM */

void inicializar_lista_bs(){
	int tamanio_memoria = leer_tamano_memoria();

	lista_particiones_bs = list_create();
	t_particion_bs* primera_part = malloc(sizeof(t_particion_bs));
	primera_part->inicio = 0;
	primera_part->fin = tamanio_memoria; //TODO ver si usar el -1
	primera_part->potencia_de_dos=obtener_potencia_de_dos_mas_cercana(tamanio_memoria);
	primera_part->libre = true;

}

int obtener_posicion_bs(int tamanio) {

	int potencia_de_dos_mas_cercana = obtener_potencia_de_dos_mas_cercana(tamanio);

	pthread_mutex_lock(&mutex_memoria_principal);

	int posicion_de_particion_en_lista = obtener_posicion_particion_mas_cercana(potencia_de_dos_mas_cercana);

	if(posicion_de_particion_en_lista == -1) {
		// TODO REALIZAR SUSTITUCION
	} else {
		t_particion_bs* posible_particion = list_get(lista_particiones_bs, posicion_de_particion_en_lista);
		if(posible_particion->potencia_de_dos == potencia_de_dos_mas_cercana) {
			// TODO OCUPAR LA PARTICION

		} else {
			// DIVIDIR EN DOS HASTA LLEGAR A POTENCIA DE DOS MAS CERCANA
			posible_particion = particionar_y_obtener_particion(posicion_de_particion_en_lista, potencia_de_dos_mas_cercana);

			// TODO OCUPAR LA PARTICION
		}
	}

	pthread_mutex_unlock(&mutex_memoria_principal);
	return -1;
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

t_particion_bs* particionar_y_obtener_particion(int posicion_a_particionar, int potencia_de_dos_deseada) {
	t_particion_bs* particion_a_particionar = list_get(lista_particiones_bs, posicion_a_particionar);

	/* TODO DESCOMENTAR Y VER PORQUE NO ME RECONOCE EL pow (#include <math.h> EN constructor.h)
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


	list_destroy(lista_auxiliar);*/
	return particion_a_particionar;
}

