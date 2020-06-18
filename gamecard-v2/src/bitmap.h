

#ifndef BITMAP_H_
#define BITMAP_H_

#include "utils.h"

t_bitarray* bitmap;
int bitmap_file_descriptor;
int flag_bloques_libres;
int ultimo_bloque_asignado;

void crear_bitmap();
void escribir_bitmap();

#endif /* BITMAP_H_ */
