#ifndef ADMMEMORIA_H_
#define ADMMEMORIA_H_
#include "constructor.h"

t_list* lista_particiones;

typedef struct{
	int inicio;
	int fin;
	int tamanio_ocupado;
	bool libre;
}t_particion;

typedef struct{
	int inicio;
	int fin;
	int potencia_de_dos;
	bool libre;
}t_particion_bs;
/*
typedef struct{
	t_particion_bs* particion;
	t_bs_nodo* izq;
	t_bs_nodo* der;
}t_bs_nodo;
*/

t_bs_nodo* arbol_bs;


/*Funciones particiones dinamicas */
void inicializar_lista_particiones();
int algoritmo_best_fit(int tamanio);
bool esta_libre(t_particion* particion);

// Funciones Buddy System
void inicializar_lista_bs();
int obtener_posicion_bs(int tamanio);
int obtener_potencia_de_dos_mas_cercana(int valor);
t_bs_nodo* obtener_nodo_mas_cercano(t_bs_nodo* nodo_a_evaluar, int potencia_de_dos);

#endif
