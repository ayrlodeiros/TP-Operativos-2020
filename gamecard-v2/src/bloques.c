#include "bloques.h"

//guarda la data en el archivo de la url
void escribir_bloque(char *path_dato,char *dato){

	char* path_directorio_blocks = devolver_path_directorio("/Blocks");
	char *path_bloque;
	int sizeOfSemiCompleteBlock = b_freeSizeOfLastBlock(url);

	//en el ultimo bloque -NO- hay espacio para guardar toda la info
	if(! (sizeOfSemiCompleteBlock >= strlen(data))){
		int blocksNeeded;
		if( ((strlen(data)-sizeOfSemiCompleteBlock) % getSizeOfBlocks()) == 0 ){
			//si entra justo le doy los bloques justos
			blocksNeeded = (strlen(data)-sizeOfSemiCompleteBlock) / getSizeOfBlocks();
		}else{
			//si no entra justo le doy un bloque demas
			blocksNeeded = (strlen(data)-sizeOfSemiCompleteBlock) / getSizeOfBlocks() + 1;
		}

		//le asigno todos los bloques que necesita
		for(int i = 0; i < blocksNeeded; i++){b_addNewBlock(url);}

	}
	//en el ultimo bloque hay espacio suficiente para guardar la info completa
	else{
		blockUrl = string_new();
		string_append(&blockUrl, blocksDirectory);
		char *x = string_itoa(b_get_lastBlock(url));
		string_append(&blockUrl, x);
		free(x);
		string_append(&blockUrl, ".bin");
		b_saveIntoBlock(blockUrl,data);

		b_updateSize(url);

		free(blockUrl);
		free(blocksDirectory);
		return;
	}
	//aca llega solo si no entro en el "else" de arriba
	int flag = 1;
	int lastPosInserted = 0;

	blockUrl = string_new();
	string_append(&blockUrl, blocksDirectory);
	char *x = string_itoa(b_get_firstFreeBlock(url));
	string_append(&blockUrl, x);
	string_append(&blockUrl, ".bin");
	free(x);

	//lleno el bloque que estaba semicompleto
	char *toInsert = string_substring(data, lastPosInserted, sizeOfSemiCompleteBlock);
	b_saveIntoBlock(blockUrl, toInsert);
	free(toInsert);
	free(blockUrl);

	lastPosInserted = sizeOfSemiCompleteBlock;

	while(flag){
		toInsert = string_new();
		if( (strlen(data)-lastPosInserted) > getSizeOfBlocks() ){ //si lo que queda no entra en un bloque
			toInsert = string_substring(data, lastPosInserted,getSizeOfBlocks());
			blockUrl = string_new();
			string_append(&blockUrl, blocksDirectory);
			char *x = string_itoa(b_get_firstFreeBlock(url));
			string_append(&blockUrl, x);
			string_append(&blockUrl, ".bin");

			b_saveIntoBlock(blockUrl,toInsert);

			lastPosInserted += getSizeOfBlocks();
			free(toInsert); free(blockUrl); free(x);
		}
		else{// si lo que queda entra en un bloque
			toInsert = string_substring_from(data, lastPosInserted);

			blockUrl = string_new();
			string_append(&blockUrl, blocksDirectory);
			char *x = string_itoa(b_get_firstFreeBlock(url));
			string_append(&blockUrl,x);
			string_append(&blockUrl, ".bin");

			b_saveIntoBlock(blockUrl, toInsert);

			free(blockUrl); free(toInsert); free(x);
			flag = 0; //condicion de corte, porque no queda mas nada que agregar
		}//else

	}//while

b_updateSize(url);

free(blocksDirectory);
}

int tamanio_libre_del_ultimo_bloque(char* path){

}

//devuelve el ultimo bloque, osea el que tiene espacio probablemente
int devolver_ultimo_bloque(char *path){
	char *bloques = devolver_lista_de_bloques(path);
	char **lista_bloques = string_get_string_as_array(bloques);
	free(bloques);
	int last = sizeofArray(lista_bloques) - 1;
	char *ultimo_bloque = string_duplicate(lista_bloques[last]);
	int numero_ultimo_bloque = atoi(ultimo_bloque);
	free(ultimo_bloque);

	for(int i = 0; i < sizeofArray(lista_bloques); i++){
		free(lista_bloques[i]);
	}
	free(lista_bloques);

	return numero_ultimo_bloque;
}

char* devolver_lista_de_bloques(char *partUrl)
{
	pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
	t_config *partition = config_create(partUrl);
	char *blocks = string_duplicate(config_get_string_value(partition,"BLOCKS"));
	config_destroy(partition);
	pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);
	return blocks;
}
