#ifndef ADMMEMORIA_H_
#define ADMMEMORIA_H_
#include "constructor.h"

typedef struct{
	int inicio;
	int fin;
	int tamanio_ocupado;
	bool libre;
}t_particion;

t_list* lista_particiones;


void inicializar_lista_particiones();
int algoritmo_best_fit(int tamanio);

#endif
