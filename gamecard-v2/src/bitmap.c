#include "bitmap.h"

void crear_bitmap(){

	int bloques = obtener_blocks();

	char* path_bitmap = devolver_path_bitmap();
	bitmap_file_descriptor = open(path_bitmap,O_RDWR | O_CREAT ,S_IRUSR | S_IWUSR);

	ftruncate(bitmap_file_descriptor,bloques/8+1);

	if(bitmap_file_descriptor == -1){
		log_error(nuestro_log,"Error al abrir el bitmap");
		close(bitmap_file_descriptor);
		//free(path_bitmap);
		return;
	}

	char* contenido_del_bitmap = mmap(NULL,bloques/8+1, PROT_READ | PROT_WRITE,MAP_SHARED,bitmap_file_descriptor,0);
	bitmap = bitarray_create_with_mode(contenido_del_bitmap,bloques/8+1, LSB_FIRST);

	//free(path_bitmap);

	for(int i = 0; i< bloques ; i++){
		bitarray_set_bit(bitmap,i);
	}

	msync(bitmap->bitarray,bitmap_file_descriptor,MS_SYNC);


}

// si no hay bloques libres devuelve -1
//devuelve un boque libre y lo marca como usado en el bitarray
int obtener_nuevo_bloque(){

if(!flag_bloques_libres){ //si no hay bloques libres ni busca
	return -1;
}

//pthread_mutex_lock(&MUTEX_BITARRAY);

int bloques = obtener_blocks();
int bloque_aux = ultimo_bloque_asignado;
int i;

for(i = ultimo_bloque_asignado; i < bloques; i++){
	if(!bitarray_test_bit(bitmap,bloque_aux)){
		bitarray_set_bit(bitmap,bloque_aux);
		ultimo_bloque_asignado = bloque_aux;
		escribir_bloque_asignado(bloque_aux);
		msync(bitmap->bitarray, bitmap_file_descriptor, MS_SYNC);
		//pthread_mutex_unlock(&MUTEX_BITARRAY);
		return bloque_aux;
	}
	else bloque_aux++;//vas al proximo bloque
}

bloque_aux = 0;
//	i=0;
while(bloque_aux < ultimo_bloque_asignado){
	if(!bitarray_test_bit(bitmap,bloque_aux)){
		bitarray_set_bit(bitmap,bloque_aux);
		ultimo_bloque_asignado = bloque_aux;
		msync(bitmap->bitarray, bitmap_file_descriptor, MS_SYNC);
		//pthread_mutex_unlock(&MUTEX_BITARRAY);
		return bloque_aux;
	}
	else bloque_aux++;
//		i++;
}
flag_bloques_libres = 0; // 0 si no hay libres, 1 si los hay
//pthread_mutex_unlock(&MUTEX_BITARRAY);
return -1; // salio del while, por lo que no hay bloque libres/
}

void escribir_bloque_asignado(int bloque){
	char* path_bloque = devolver_path_dato(bloque);

	FILE* archivo = txt_open_for_append(path_bloque);
	txt_write_in_file(archivo,"&");
	txt_close_file(archivo);

	free(path_bloque);
}

void destruir_bloque(){
	msync(bitmap->bitarray, bitmap_file_descriptor, MS_SYNC);
	bitarray_destroy(bitmap);
}

//libera el bloque
void liberar_bloque(int bloque){
	//pthread_mutex_lock(&MUTEX_BITARRAY);

	bitarray_clean_bit(bitmap,bloque);
	flag_bloques_libres = 1;

	//pthread_mutex_unlock(&MUTEX_BITARRAY);
}


