#include "bloques.h"

//guarda la data en el archivo del path
void escribir_bloque(char* path_config,char* dato){

	char* path_bloque;
	char* path_directorio_bloque = devolver_path_directorio("/Blocks");
	int tamanio_disponible_del_ultimo_bloque = tamanio_libre_del_ultimo_bloque(path_config);
	log_info(nuestro_log,"Tamanio Disponible:%d",tamanio_disponible_del_ultimo_bloque);

/*
	t_config* config_pokemon = config_create(path_config);

	if(list_is_empty(obtener_blocks_archivo_metadata_pokemon("Pikachu"))){
		guardar_en_bloque(obtener_primer_bloque_libre("kgaas"),posicion,cantidad);
	}
*/
	//en el ultimo bloque -NO- hay espacio para guardar toda la info
	int tamanio_dato = strlen(dato);
	log_info(nuestro_log,"Tamaño dato :%d",tamanio_dato);
	log_info(nuestro_log,"Tamaño disponible :%d",tamanio_disponible_del_ultimo_bloque);
	if(tamanio_disponible_del_ultimo_bloque < tamanio_dato){
		int bloques_necesarios;
		if( ((tamanio_dato-tamanio_disponible_del_ultimo_bloque) % obtener_block_size()) == 0 ){
			//si entra justo le doy los bloques justos
			bloques_necesarios = (tamanio_dato-tamanio_disponible_del_ultimo_bloque) / obtener_block_size();
		}else{
			//si no entra justo le doy un bloque demas
			bloques_necesarios = (tamanio_dato-tamanio_disponible_del_ultimo_bloque) / obtener_block_size() + 1;
		}
		log_info(nuestro_log,"Bloques Necesarios :%d",bloques_necesarios);
		//le asigno todos los bloques que necesita
		for(int i = 0; i < bloques_necesarios; i++){
			agregar_bloque(path_config);

		}
	}
	//en el ultimo bloque hay espacio suficiente para guardar la info completa
	else{
		int ultimo_bloque = devolver_ultimo_bloque(path_config);
		escribir_bloque_asignado(ultimo_bloque);
		char* string_dato = string_itoa(devolver_ultimo_bloque(path_config));
		path_bloque = devolver_path_dato(string_dato);
		//agregar_bloque(path_config); //REVISAR ESTO
		guardar_en_bloque(path_bloque,dato);
		actualizar_tamanio_bloque(path_config);
		free(path_bloque);
		free(path_directorio_bloque);
		return;
	}
	log_info(nuestro_log,"PRUEBA 1");

	//aca llega solo si no entro en el "else" de arriba
	int flag = 1;
	int ultima_posicion_insertada = 0;
	log_info(nuestro_log,"PRUEBA 2");
	log_info(nuestro_log,"Bloques : %s",devolver_lista_de_bloques(path_config));
	int bloque_a_escribir = obtener_primer_bloque_libre(path_config);
	path_bloque = devolver_path_dato(string_itoa(bloque_a_escribir));
	//lleno el bloque que estaba semicompleto
	char* a_escribir = string_substring(dato, ultima_posicion_insertada, tamanio_disponible_del_ultimo_bloque);
	char* restante = string_substring_from(dato, strlen(a_escribir));
	log_info(nuestro_log,"DATO :%s",dato);
	log_info(nuestro_log,"A_escribir :%s",a_escribir);
	log_info(nuestro_log,"Restante :%s",restante);



	guardar_en_bloque(path_bloque,a_escribir);
	free(a_escribir);
	ultima_posicion_insertada = tamanio_disponible_del_ultimo_bloque;
	while(flag){

		tamanio_dato = strlen(dato);

		if(tamanio_dato >= tamanio_disponible_del_ultimo_bloque){ //si lo que queda no entra en un bloque

			a_escribir = string_new();
			a_escribir = string_substring(dato, ultima_posicion_insertada, tamanio_disponible_del_ultimo_bloque);
			log_info(nuestro_log,"Agregue bloque asd");
			log_info(nuestro_log,"Agregue bloque 0-0");
			path_bloque = devolver_path_dato(string_itoa(obtener_primer_bloque_libre(path_config)));
			log_info(nuestro_log,"Agregue bloque 0");
			guardar_en_bloque(path_bloque,restante);

			ultima_posicion_insertada += obtener_block_size();
			free(restante);
			free(path_bloque);
			log_info(nuestro_log,"Agregue bloque 1");

		}
		else{// si lo que queda entra en un bloque

			restante = string_substring_from(dato, strlen(a_escribir));
			path_bloque = devolver_path_dato(string_itoa(obtener_primer_bloque_libre(path_config)));

			guardar_en_bloque(path_bloque,restante);

			free(path_bloque);
			free(a_escribir);
			flag = 0; //condicion de corte, porque no queda mas nada que agregar
		}//else

	}//while
	actualizar_tamanio_bloque(path_config);

free(path_directorio_bloque);
}

void agregar_bloques_al_metadata(char* path_nombre_metadata,int tamanio_dato,int tamanio_disponible_del_ultimo_bloque){
	int bloques_necesarios;
	if( ((tamanio_dato-tamanio_disponible_del_ultimo_bloque) % obtener_block_size()) == 0 ){
	//si entra justo le doy los bloques justos
	bloques_necesarios = (tamanio_dato-tamanio_disponible_del_ultimo_bloque) / obtener_block_size();
	}else{
	//si no entra justo le doy un bloque demas
	bloques_necesarios = (tamanio_dato-tamanio_disponible_del_ultimo_bloque) / obtener_block_size() + 1;
	}
	//le asigno todos los bloques que necesita
	for(int i = 0; i < bloques_necesarios; i++){
		agregar_bloque(path_nombre_metadata);
		bitarray_set_bit(bitmap,i);
	}
}

void agregar_dato_al_bloque (char* path_nombre_metadata,char* dato_a_escribir){
	int ultimo_bloque = devolver_ultimo_bloque(path_nombre_metadata);
	bitarray_set_bit(bitmap,ultimo_bloque);
	char* string_dato = string_itoa(devolver_ultimo_bloque(path_nombre_metadata));
	char* path_bloque = devolver_path_dato(string_dato);
	guardar_en_bloque(path_bloque,dato_a_escribir);
	actualizar_tamanio_bloque(path_nombre_metadata);
	free(path_bloque);
}

void escribir_bloque_v2(char* path_nombre_metadata,char* dato){
	int tamanio_disponible_del_ultimo_bloque = tamanio_libre_del_ultimo_bloque(path_nombre_metadata);
	char* dato_a_escribir = string_new();
	string_append(&dato_a_escribir,dato);
	string_append(&dato_a_escribir,"/");
	int tamanio_dato = strlen(dato_a_escribir);

	//Si el dato no entra en el bloque
	if(tamanio_dato > tamanio_disponible_del_ultimo_bloque - 1){
		agregar_bloques_al_metadata(path_nombre_metadata,tamanio_dato,tamanio_disponible_del_ultimo_bloque);

		int bloque_a_escribir = obtener_primer_bloque_libre(path_nombre_metadata);
		char* path_bloque_a_escribir = devolver_path_dato(string_itoa(bloque_a_escribir));
		char* a_escribir = string_new();
		a_escribir = string_substring(dato_a_escribir, 0, tamanio_disponible_del_ultimo_bloque);
		guardar_en_bloque(path_bloque_a_escribir,a_escribir);

		actualizar_tamanio_bloque(path_nombre_metadata);

		char* dato_a_escribir_restante = string_new();
		dato_a_escribir_restante = string_substring_from(dato_a_escribir, strlen(a_escribir));
		int tamanio_dato_a_escribir_restante = strlen(dato_a_escribir_restante);
		int bloque_nuevo_a_escribir = obtener_primer_bloque_libre(path_nombre_metadata);
		char* path_bloque_nuevo_a_escribir = devolver_path_dato(string_itoa(bloque_nuevo_a_escribir));

		guardar_en_bloque(path_bloque_nuevo_a_escribir,dato_a_escribir_restante);

		actualizar_tamanio_bloque(path_nombre_metadata);
		free(a_escribir);
		free(dato_a_escribir_restante);
	}
	//en el ultimo bloque hay espacio suficiente para guardar la info completa
	else{
		agregar_dato_al_bloque(path_nombre_metadata,dato_a_escribir);
	}
	free(dato_a_escribir);
}

void escribir_bloque_asignado(int bloque){
	char* path_bloque = devolver_path_dato(string_itoa(bloque));
	bitarray_set_bit(bitmap,bloque);


	FILE* archivo = txt_open_for_append(path_bloque);
	txt_write_in_file(archivo,"&");
	txt_close_file(archivo);

	free(path_bloque);

}

//limpio el contenido del bloque y lo libero en el bitarray
void limpiar_bloque(int bloque){
	char* path_bloque = devolver_path_dato(bloque);

	FILE *archivo = fopen(path_bloque,"w"); //limpio el archivo
	fclose(archivo);

	liberar_bloque(bloque); //libero en bitarray

	free(path_bloque);
}


int se_creo_el_bloque(){
	/*char* path_bloque = devolver_path_directorio("/Blocks");
	string_append(&path_bloque,"0.bin");

	if(access(path_bloque,F_OK) != -1){
		free(path_bloque);
		return true;
	}
	else {
		free(path_bloque);
		return false;
	}*/
}

void crear_bloque(){ //ANDA BIEN
	char* path_bloque;

	for(int i = 0; i < obtener_blocks(); i++){
		path_bloque = devolver_path_dato(string_itoa(i));
		FILE* archivo = fopen(path_bloque,"w");
		fclose(archivo);
		free(path_bloque);
	}

}

void modificar_tamanio_bloque(char* path_bloque,int tamanio){
	//pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
	t_config* config = config_create(path_bloque);
	char* tamanio_string = string_itoa(tamanio);
	config_set_value(config,"SIZE",tamanio_string);
	config_save(config);
	config_destroy(config);
	free(tamanio_string);
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);
}

//actualiza el tamano
void actualizar_tamanio_bloque(char* path_bloque){
	log_info(nuestro_log,"Path_bloque :%s",path_bloque);
	char* string_lista_bloques = devolver_lista_de_bloques(path_bloque);
	char** lista_de_bloques = string_get_string_as_array(string_lista_bloques);
	free(string_lista_bloques);

	int tamanio = (tamanio_de_lista(lista_de_bloques) - 1) * obtener_block_size(); //no deberia llegarle nunca un array tam 0
	tamanio += (obtener_block_size() - tamanio_libre_del_ultimo_bloque(path_bloque));
	modificar_tamanio_bloque(path_bloque,tamanio);

	for(int i = 0; i < tamanio_de_lista(lista_de_bloques); i++){
		free(lista_de_bloques[i]);
	}
	free(lista_de_bloques);
}

//guarda en el path del bloque lo que se le pasa por parametro
//esta funcion no deberia romper nunca por overflow de tamano de bloque porquese cheuquea antes de usarla
void guardar_en_bloque(char* path_bloque, char* dato){

	/*
	char* path_bloque = devolver_path_dato(string_itoa(bloque));
	t_config* archivo_pokemon = config_create(path_bloque);
	config_set_value(archivo_pokemon,key_a_guardar,nueva_cantidad);
	config_save(archivo_pokemon);
	bitarray_set_bit(bitmap,bloque);
	*/
	char* pivot = malloc(3);
	struct stat st;
	FILE* archivo;
	stat(path_bloque,&st);
	int tamanio_archivo = st.st_size;

	if(tamanio_archivo == 1){
		archivo = fopen(path_bloque,"r");
		fread(pivot,tamanio_archivo,1,archivo);
		fclose(archivo);
		pivot[tamanio_archivo] = '\0';
		if(!strcmp(pivot,"&")){
			archivo = fopen(path_bloque,"w");
			fclose(archivo);
		}
	}

	FILE* archivo2 = txt_open_for_append(path_bloque);
	txt_write_in_file(archivo2, dato);
	txt_close_file(archivo2);
	free(pivot);

}

int tamanio_libre_real(int bloque){
	char* path_bloque = devolver_path_dato(string_itoa(bloque));

		struct stat st;
		stat(path_bloque,&st);

		int tamanio_actual = st.st_size;

		free(path_bloque);
		return (obtener_block_size() - tamanio_actual);

}

int tamanio_libre_del_bloque(int bloque){
	char* bloque_aux = string_itoa(bloque);
	char* path = devolver_path_dato(bloque_aux);

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

	int nuevo_bloque = obtener_nuevo_bloque_libre();
	char* lista_bloques_string = devolver_lista_de_bloques(path_bloque);
	char** lista_bloques = string_get_string_as_array(lista_bloques_string);

	t_list* t_list_bloques = list_create();
	for(int j = 0; j < tamanio_de_lista(lista_bloques); j++){
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

//no deberia salir del for, osea tiene que tener siempre asignado un bloque libre al menos
//-1 si sale del for, pero nodeberia pasar
//te devuelve el primer bloque libre de la tabla
int obtener_primer_bloque_libre(char* path_bloque){
	char* lista_bloques_string = devolver_lista_de_bloques(path_bloque);
	char** lista_de_bloques = string_get_string_as_array(lista_bloques_string);
	int valor_en_int;
	free(lista_bloques_string);
	for(int i = 0; i < tamanio_de_lista(lista_de_bloques); i++){
		if(!el_bloque_esta_lleno(strtol(lista_de_bloques[i],NULL,10))){
			valor_en_int = strtol(lista_de_bloques[i],NULL,10);
			for(int j = 0; j < tamanio_de_lista(lista_de_bloques); j++){
				free(lista_de_bloques[j]);
			}
			free(lista_de_bloques);
			return valor_en_int;
		}
	}

	for(int j = 0; j < tamanio_de_lista(lista_de_bloques); j++){
		free(lista_de_bloques[j]);
	}
	free(lista_de_bloques);

	return -1;
	/*
	for(int i= 0; i< obtener_blocks();i++){
		if(!bitarray_test_bit(bitmap,i)){
			return i;
		}
		log_error(nuestro_log,"no hay bloques libres");
		return -1;
	}*/
}

int el_bloque_esta_lleno(int bloque){
	return tamanio_libre_del_bloque(bloque) == 0;
}

int el_bloque_esta_vacio(int bloque){
	return tamanio_libre_real(bloque) == obtener_block_size();
}

int tamanio_libre_del_ultimo_bloque(char* path){
	int ultimo_bloque = devolver_ultimo_bloque(path);
	return tamanio_libre_del_bloque(ultimo_bloque);
}

//devuelve el ultimo bloque, osea el que tiene espacio probablemente
int devolver_ultimo_bloque(char* path){
	char* bloques = devolver_lista_de_bloques(path);
	char** lista_bloques = string_get_string_as_array(bloques);
	free(bloques);
	//if(string_length(lista_bloques) > 0){
		int posicion_ultimo_bloque = tamanio_de_lista (lista_bloques) - 1;
		char* ultimo_bloque = string_duplicate(lista_bloques[posicion_ultimo_bloque]);
		int numero_ultimo_bloque = strtol(ultimo_bloque,NULL,10);
		free(ultimo_bloque);
		for(int i = 0; i < tamanio_de_lista(lista_bloques); i++){
			free(lista_bloques[i]);
		}
		free(lista_bloques);
		return numero_ultimo_bloque;
	//}else{
	//	return obtener_primer_bloque_libre(path);
	//}
}

int tamanio_de_lista(char** un_array){
	int i = 0;

	if(!un_array[i]){
		return 0;
	}

	while(un_array[i] != NULL){
		i++;
	}
	return i;
}


char* devolver_lista_de_bloques(char* path_files_config){
	//pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
	t_config* config_file = config_create(path_files_config);
	char* bloques = string_duplicate(config_get_string_value(config_file,"BLOCKS"));
	config_destroy(config_file);
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);
	return bloques;
}


void empezar_file_metadata(char* path_archivo_files_metadata,char* es_directorio,int bloque, int tamanio, char* esta_abierto){
	//pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
	char *dato_a_guardar = string_new();

	string_append(&dato_a_guardar,"DIRECTORY=");
	string_append(&dato_a_guardar,es_directorio);
	string_append(&dato_a_guardar,"SIZE=");
	char* string_tamanio = string_itoa(tamanio);
	string_append(&dato_a_guardar,string_tamanio);
	string_append(&dato_a_guardar,"\n");
	string_append(&dato_a_guardar,"BLOCKS=[");
	char* string_bloque = string_itoa(bloque);
	string_append(&dato_a_guardar,bloque);
	free(string_bloque);
	string_append(&dato_a_guardar,"]\n");
	string_append(&dato_a_guardar,"OPEN=");
	string_append(&dato_a_guardar,esta_abierto);

	FILE* archivo = txt_open_for_append(path_archivo_files_metadata);
	txt_write_in_file(archivo,dato_a_guardar);
	txt_close_file(archivo);
	free(dato_a_guardar);
	free(string_tamanio);
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);
}

void cargar_datos_del_file_metadata (char* path_pokemon){

	char* path = devolver_path_files_metadata(path_pokemon);
	string_append(&path,"/Metadata.bin");

	asignar_tamanio_y_bloque(path,0);

	free(path);
}

void asignar_tamanio_y_bloque (char* path,int tamanio){
	int bloque = obtener_nuevo_bloque_libre();

	empezar_file_metadata(path,"N",bloque,0,"N");
}

