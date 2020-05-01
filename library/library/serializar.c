
#include "serializar.h"


void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	void * magic = malloc(bytes);
	int offset = 0;

	/*memcpy(magic + offset, &(paquete->codigo_operacion),sizeof(int));
	offset += sizeof(int);
	memcpy(magic + offset,&(paquete->buffer->size),sizeof(int));
	offset += sizeof(int);
	memcpy(magic + offset,paquete->buffer->stream, paquete->buffer->size);*/

	return magic;
}
