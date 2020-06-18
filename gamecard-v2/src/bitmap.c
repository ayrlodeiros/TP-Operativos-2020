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
/*
void escribir_bitmap(){

	int file_descriptor = open(devolver_path_bitmap(),O_WRONLY,S_IRUSR | S_IWUSR);

	struct stat sb;
	fstat(file_descriptor,&sb);
	//ftruncate()

	bitmap = bitarray_create(bitmap,obtener_blocks()/8+1);

	log_info(nuestro_log,"sb size %d",obtener_blocks()/8+1);
	char* file_in_memory = mmap(NULL,obtener_blocks() * obtener_block_size(),PROT_WRITE,MAP_SHARED,file_descriptor,0);
	log_info(nuestro_log,"PRUEBA");
	for(int i = 0; i < sb.st_size;i++){
		log_info(nuestro_log,"PRUEBA : %d",i);
		memset(file_in_memory,0,obtener_blocks()/8);

	}
	log_info(nuestro_log,"FILE MEMORY");
	for(int i = 0; i < 10;i++){
		log_info(nuestro_log,"VALOR BIT :%d",(int)bitarray_test_bit(file_in_memory,i));
	}

	munmap(file_in_memory,sb.st_size);

}
*/
