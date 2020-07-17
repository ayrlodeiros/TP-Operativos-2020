#include "bitmap.h"

void crear_bitmap(){

	pthread_mutex_lock(&Mutex_Bitmap);

	int bloques = obtener_blocks();

	char* path_bitmap = devolver_path_bitmap();
	bitmap_file_descriptor = open(path_bitmap,O_RDWR | O_CREAT ,S_IRUSR | S_IWUSR);

	ftruncate(bitmap_file_descriptor,bloques/8+1);

	if(bitmap_file_descriptor == -1){
		log_error(nuestro_log,"Error al abrir el bitmap");
		close(bitmap_file_descriptor);
		free(path_bitmap); //Agregue 1 free en bitmap.c linea 17 y anda
		return;
	}

	char* contenido_del_bitmap = mmap(NULL,bloques/8+1, PROT_READ | PROT_WRITE,MAP_SHARED,bitmap_file_descriptor,0);
	bitmap = bitarray_create_with_mode(contenido_del_bitmap,bloques/8+1, LSB_FIRST);

	free(path_bitmap);
	msync(bitmap->bitarray,bitmap_file_descriptor,MS_SYNC);

	log_info(nuestro_log,"TERMINE DE CREAR BITMAP");

	pthread_mutex_unlock(&Mutex_Bitmap);
}

// si no hay bloques libres devuelve -1
//devuelve un boque libre y lo marca como usado en el bitarray
int obtener_nuevo_bloque_libre(){

	if(!flag_bloques_libres){ //si no hay bloques libres ni busca
		return -1;
	}

	pthread_mutex_lock(&Mutex_Bitmap);

	int bloques = obtener_blocks();
	int bloque_aux = ultimo_bloque_asignado;
	int i;

	for(i = 0; i < bloques; i++){
		if(!bitarray_test_bit(bitmap,i)){
			msync(bitmap->bitarray, bitmap_file_descriptor, MS_SYNC);
			pthread_mutex_unlock(&Mutex_Bitmap);
			return i;
		}
		//else bloque_aux++;//vas al proximo bloque
	}

	flag_bloques_libres = 0; // 0 si no hay libres, 1 si los hay
	pthread_mutex_unlock(&Mutex_Bitmap);
	return -1; // salio del while, por lo que no hay bloque libres/
}

void destruir_bitmap(){
	msync(bitmap->bitarray, bitmap_file_descriptor, MS_SYNC);
	bitarray_destroy(bitmap);
}



