#ifndef ADMMEMORIA_H_
#define ADMMEMORIA_H_
#include "constructor.h"
#include <sys/time.h>

int orden_de_llegada;
t_list* lista_particiones_dinamicas;
t_list* lista_particiones_bs;

typedef struct{
	int inicio;
	int fin;
	int tamanio_ocupado;
	uint64_t tiempo_ingreso;
	uint64_t ult_vez_usado;
	bool libre;
}t_particion_dinamica;

typedef struct{
	int inicio;
	int fin;
	int potencia_de_dos;
	uint64_t tiempo_ingreso;
	uint64_t ult_vez_usado;
	bool libre;
}t_particion_bs;


/*Funciones particiones dinamicas */

void inicializar_lista_particiones();

int obtener_pos_particiones(int tamanio);
int buscar_particion_libre(int tamanio);
void liberar_particion();
bool estaOcupado(void* elemento);

int llenar_y_realizar_nueva_particion(t_particion_dinamica* particion,int tamanio,int posicion_en_lista);
int llenar_particion(t_particion_dinamica* particion, int tamanio);

int algoritmo_best_fit(int tamanio);
int algoritmo_first_fit(int tamanio);

int algoritmo_reemplazo_fifo();
int algoritmo_reemplazo_lru();

bool particion_libre_a_la_izquierda(int posicion);
bool particion_libre_a_la_derecha(int posicion);

int diferencia_tamanio_particion(t_particion_dinamica* particion,int tamanio_msj);
int tamanio_particion(t_particion_dinamica* particion);

bool esta_libre(t_particion_dinamica* particion);
bool hay_que_compactar();
void compactacion();
void consolidar(int pos_particion);
void borrar_msj_mp(int posicion, int tamanio);

/* Esto es para cuando se accede a la memoria de una particion, saber en que momento fue utilizada para el algoritmo LRU */
uint64_t timestamp(void);

/* Funciones Buddy System */

void inicializar_lista_bs();
int obtener_posicion_bs(int tamanio);
int obtener_potencia_de_dos_mas_cercana(int valor);
int obtener_posicion_particion_mas_cercana(int potencia_de_dos);
t_particion_bs* particionar_y_obtener_particion(int posicion_a_particionar, int potencia_de_dos_deseada);

#endif
