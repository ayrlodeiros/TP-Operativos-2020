#ifndef ADMMEMORIA_H_
#define ADMMEMORIA_H_
#include "constructor.h"


int contador_compactacion;
t_list* lista_particiones;
t_list* lista_particiones_bs;

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


/* Funciones Buddy System */

void inicializar_lista_bs();
int obtener_posicion_bs(int tamanio);
int obtener_potencia_de_dos_mas_cercana(int valor);
int obtener_posicion_particion_mas_cercana(int potencia_de_dos);
t_particion_bs* particionar_y_obtener_particion(int posicion_a_particionar, int potencia_de_dos_deseada);

#endif
