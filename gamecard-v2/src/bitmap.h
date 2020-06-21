

#ifndef BITMAP_H_
#define BITMAP_H_

#include "utils.h"

t_bitarray* bitmap;
int bitmap_file_descriptor;
int flag_bloques_libres;
int ultimo_bloque_asignado;

void crear_bitmap();
void escribir_bitmap();
void destruir_bloque();
void liberar_bloque(int bloque);
void escribir_bloque_asignado(int bloque);

#endif /* BITMAP_H_ */