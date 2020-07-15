#ifndef ADMMEMORIA_H_
#define ADMMEMORIA_H_
#include "constructor.h"


int contador_compactacion;
t_list* lista_particiones;

typedef struct{
	int inicio;
	int fin;
	int tamanio_ocupado;
	//todo chequear que funcion asi
	bool libre = true;
}t_particion;

typedef struct{
	int inicio;
	int fin;
	int potencia_de_dos;
	bool libre;
}t_particion_bs;

typedef struct{
	t_particion_bs* particion;
	t_bs_nodo* izq;
	t_bs_nodo* der;
}t_bs_nodo;


t_bs_nodo* arbol_bs;


/*Funciones particiones dinamicas */


void inicializar_lista_particiones();
void inicializar_contador_compactacion();

int obtener_pos_particiones(int tamanio);
int buscar_particion_libre(int tamanio);
void liberar_particion();

int algoritmo_best_fit(int tamanio);
int algoritmo_first_fit(int tamanio);

bool esta_libre(t_particion* particion);
bool hay_que_compactar();
void compactacion();

// Funciones Buddy System
void inicializar_lista_bs();
int obtener_posicion_bs(int tamanio);
int obtener_potencia_de_dos_mas_cercana(int valor);
t_bs_nodo* obtener_nodo_mas_cercano(t_bs_nodo* nodo_a_evaluar, int potencia_de_dos);

#endif
