#include "bloques.h"

//guarda la data en el archivo de la url
void escribir_bloque(char *path_dato,char *dato){

	char* path_directorio_blocks = devolver_path_directorio("/Blocks");
	char *path_bloque;
	int tamanio_disponible_del_ultimo_bloque = tamanio_libre_del_ultimo_bloque(path_bloque);

	//en el ultimo bloque -NO- hay espacio para guardar toda la info
	if(! (tamanio_disponible_del_ultimo_bloque >= strlen(dato))){

		int bloques_necesarios;

		if( ((strlen(dato)-tamanio_disponible_del_ultimo_bloque) % obtener_block_size()) == 0 ){
			//si entra justo le doy los bloques justos
			bloques_necesarios = (strlen(dato)-tamanio_disponible_del_ultimo_bloque) / obtener_block_size();
		}else{
			//si no entra justo le doy un bloque demas
			bloques_necesarios = (strlen(dato)-tamanio_disponible_del_ultimo_bloque) / obtener_block_size() + 1;
		}

		//le asigno todos los bloques que necesita
		for(int i = 0; i < bloques_necesarios; i++){
			b_addNewBlock(path_bloque);
		}

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

int tamanio_libre_del_bloque(int bloque){
	char* path = devolver_path_dato(bloque);

		struct stat st;
		stat(path,&st);

		int tamanio_actual = st.st_size;

		if(tamanio_actual == 1){ //me fijo si lo que tiene es "&"
			FILE *archivo = fopen(path,"r");
			char *pivot = malloc(3);
			fread(pivot,1,1,archivo);
			fclose(archivo);
			pivot[tamanio_actual] = '\0';
			if(!strcmp(pivot,"&"))
				tamanio_actual = 0;
			free(pivot);
		}

		free(path);

		return (obtener_block_size() - tamanio_actual);
}

//al archivo le agrego un nuevo bloque a la lista de bloques
void agregar_bloque(char* path_bloque){

	int nuevo_bloque = obtener_nuevo_bloque();
	char* lista_bloques_string = devolver_lista_de_bloques(path_bloque);
	char** lista_bloques = string_get_string_as_array(lista_bloques_string);

	t_list* t_list_bloques = list_create();
	for(int j = 0; j < sizeofArray(lista_bloques); j++){
		list_add(t_list_bloques,string_duplicate(lista_bloques[j]));
		free(lista_bloques[j]);
	}
	free(lista_bloques);
	free(lista_bloques_string);

	char* string_nuevo_bloque = string_itoa(nuevo_bloque);
	list_add(t_list_bloques,string_nuevo_bloque);

	char* lista_de_string = string_new();
	string_append(&lista_de_string, "[");
	for(int k = 0; k < list_size(t_list_bloques); k++){
		string_append(&lista_de_string,list_get(t_list_bloques,k));
		if((k + 1) != list_size(t_list_bloques)) string_append(&lista_de_string,",");
	}
	string_append(&lista_de_string, "]");

	modificar_bloque(path_bloque,lista_de_string);

	list_destroy_and_destroy_elements(t_list_bloques,free);

	free(lista_de_string);
}

//pisa el valor de BLOCKS por el de listBlocks
void modificar_bloque(char* path_particion, char* lista_bloques){
	//pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
	t_config* particion = config_create(path_particion);
	config_set_value(particion,"BLOCKS",lista_bloques);
	config_save(particion);
	config_destroy(particion);
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);

}

int tamanio_libre_del_ultimo_bloque(char* path){
	int ultimo_bloque = devolver_ultimo_bloque(path);
	return tamanio_libre_del_bloque(ultimo_bloque);
}

//devuelve el ultimo bloque, osea el que tiene espacio probablemente
int devolver_ultimo_bloque(char* path){
	//falta hacer la funcion para que arranque la particion

	char* bloques = devolver_lista_de_bloques(path);
	char** lista_bloques = string_get_string_as_array(bloques);

	free(bloques);

	int posicion_ultimo_bloque = tamanio_del_array (lista_bloques) - 1;
	char* ultimo_bloque = string_duplicate(lista_bloques[posicion_ultimo_bloque]);
	int numero_ultimo_bloque = atoi(ultimo_bloque);

	free(ultimo_bloque);

	for(int i = 0; i < tamanio_del_array(lista_bloques); i++){
		free(lista_bloques[i]);
	}
	free(lista_bloques);

	return numero_ultimo_bloque;
}

int tamanio_del_array(char** un_array){
	int i = 0;

	if(!un_array[i]){
		return 0;
	}

	while(un_array[i] != NULL){
		i++;
	}

	return i;
}


char* devolver_lista_de_bloques(char* path_particion)
{
	//pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
	t_config* particion = config_create(path_particion);
	char* bloques = string_duplicate(config_get_string_value(particion,"BLOCKS"));
	config_destroy(particion);
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);
	return bloques;
}



