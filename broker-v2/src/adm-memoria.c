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




//BUDDY SYSTEM

void inicializar_lista_bs(){
	int tamanio_memoria = leer_tamano_memoria();

	arbol_bs = malloc(sizeof(t_bs_nodo));
	t_particion_bs* primera_part = malloc(sizeof(t_particion_bs));
	primera_part->inicio = 0;
	primera_part->fin = tamanio_memoria-1; //TODO ver si usar el fin
	primera_part->potencia_de_dos=obtener_potencia_de_dos_mas_cercana(tamanio_memoria);
	primera_part->libre = true;

	arbol_bs->particion = primera_part;
	arbol_bs->izq = NULL;
	arbol_bs->der = NULL;
}

int obtener_posicion_bs(int tamanio) {

	int potencia_de_dos_mas_cercana = obtener_potencia_de_dos_mas_cercana(tamanio);

	pthread_mutex_lock(&mutex_memoria_principal);

	t_bs_nodo* nodo_mas_cercano = obtener_nodo_mas_cercano(arbol_bs, potencia_de_dos_mas_cercana);
	if(nodo_mas_cercano == NULL) {
		//TODO IMPLEMENTAR ALGORITMO PARA LA SUSTICION DE ALGUNA PARTICION
	} else {
		if(nodo_mas_cercano->particion->potencia_de_dos == potencia_de_dos_mas_cercana) {
			//TODO MARCAR PARTICION COMO OCUPADA
		} else {
			//TODO PARTICIONAR ARBOL HASTA LA potencia_de_dos_mas_cercana Y DEVOLVER NODO
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

t_bs_nodo* obtener_nodo_mas_cercano(t_bs_nodo* nodo_a_evaluar, int potencia_de_dos) {

	if(nodo_a_evaluar->particion->libre) {
		if(nodo_a_evaluar->particion->potencia_de_dos >= potencia_de_dos) {
			return nodo_a_evaluar;
		} else {
			return NULL;
		}
	} else {
		t_bs_nodo* nodo_izq = obtener_nodo_mas_cercano(nodo_a_evaluar, potencia_de_dos);
		t_bs_nodo* nodo_der = obtener_nodo_mas_cercano(nodo_a_evaluar, potencia_de_dos);

		if(nodo_izq == NULL) {
			if(nodo_der == NULL) {
				return NULL;
			} else {
				return nodo_der;
			}
		} else {
			return nodo_izq;
		}
	}
}
