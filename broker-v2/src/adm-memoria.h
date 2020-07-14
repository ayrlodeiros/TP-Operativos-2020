#ifndef ADMMEMORIA_H_
#define ADMMEMORIA_H_
#include "constructor.h"

typedef enum{
	FIRST_FIT = 1,
	BEST_FIT = 2
}algor_part_libre;

typedef enum{
	FIRST_I_FIRST_O = 1,
	LAST_RECENTLY_USED = 2
}algor_reemplazo;

typedef struct{
	int inicio;
	int fin;
	bool libre;
}t_particion;


typedef struct{
	t_particion_bs* particion;
	t_bs_nodo* izq;
	t_bs_nodo* der;
}t_bs_nodo;

typedef struct{
	int inicio;
	int fin;
	int potencia_de_dos;
	bool libre;
}t_particion_bs;

t_list* lista_particiones;
t_bs_nodo* arbol_bs;


void inicializar_lista_particiones();
int algoritmo_best_fit(int tamanio);

// Funciones Buddy System
void inicializar_lista_bs();
int obtener_posicion_bs(int tamanio);
int obtener_potencia_de_dos_mas_cercana(int valor);
t_bs_nodo* obtener_nodo_mas_cercano(t_bs_nodo* nodo_a_evaluar, int potencia_de_dos);

#endif
