#include "adm-memoria.h"



void inicializar_lista_particiones(){
	t_particion* primera_part = malloc(sizeof(t_particion));
	primera_part->inicio = 0;
	primera_part->fin = leer_tamano_memoria();
	primera_part->libre = true;
	lista_particiones = list_create();
	list_add(lista_particiones,primera_part);
}

int obtener_pos_particiones(int tamanio) {
	int posicion;
	//TODO impletar obtencion posicion particiones
	pthread_mutex_lock(&mutex_memoria_principal);

	switch(leer_algoritmo_particion_libre()){
	case FF:
		break;
	case BF:
		posicion = algoritmo_best_fit(tamanio);
		break;
	}

	pthread_mutex_unlock(&mutex_memoria_principal);
	return -1;
}
//todo seguir haciendo
int algoritmo_best_fit(int tamanio){
	t_particion mejor_particion;
	for(int i = 0; list_size(lista_particiones) > i ; i++ ){

	}
	return -1;
}
