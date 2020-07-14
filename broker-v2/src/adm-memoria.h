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

t_list* lista_particiones;


void inicializar_lista_particiones();
int algoritmo_best_fit(int tamanio);

#endif
