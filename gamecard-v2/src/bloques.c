#include "bloques.h"

//guarda la data en el archivo del path
void escribir_bloque(char* path_dato,char* dato){

	char* path_directorio_bloque = devolver_path_directorio("/Blocks");
	char* path_bloque;
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
			agregar_bloque(path_bloque);
		}

	}
	//en el ultimo bloque hay espacio suficiente para guardar la info completa
	else{
		path_bloque = string_new();
		string_append(&path_bloque, path_directorio_bloque );
		char* string_ultimo_bloque = string_itoa(devolver_ultimo_bloque(path_dato));
		string_append(&path_bloque, string_ultimo_bloque);
		free(string_ultimo_bloque);
		string_append(&path_bloque, ".bin");
		guardar_en_bloque(path_bloque,dato);

		actualizar_tamanio_bloque(path_dato);

		free(path_bloque);
		free(path_directorio_bloque);
		return;
	}
	//aca llega solo si no entro en el "else" de arriba
	int flag = 1;
	int ultima_posicion_insertada = 0;


	path_bloque = devolver_path_dato(obtener_primer_bloque_libre(path_dato));

	//lleno el bloque que estaba semicompleto
	char* a_escribir = string_substring(dato, ultima_posicion_insertada, tamanio_disponible_del_ultimo_bloque);
	guardar_en_bloque(path_bloque,a_escribir);
	free(a_escribir);

	ultima_posicion_insertada = tamanio_disponible_del_ultimo_bloque;

	while(flag){
		a_escribir = string_new();
		if( (strlen(dato)-ultima_posicion_insertada) > obtener_block_size() ){ //si lo que queda no entra en un bloque
			a_escribir = string_substring(dato, ultima_posicion_insertada,obtener_block_size());

			path_bloque = devolver_path_dato(obtener_primer_bloque_libre(path_dato));

			guardar_en_bloque(path_bloque,a_escribir);

			ultima_posicion_insertada += obtener_block_size();
			free(a_escribir);
			free(path_bloque);

		}
		else{// si lo que queda entra en un bloque
			a_escribir = string_substring_from(dato, ultima_posicion_insertada);

			path_bloque = devolver_path_dato(obtener_primer_bloque_libre(path_dato));

			guardar_en_bloque(path_bloque, a_escribir);

			free(path_bloque);
			free(a_escribir);
			flag = 0; //condicion de corte, porque no queda mas nada que agregar
		}//else

	}//while

	actualizar_tamanio_bloque(path_dato);

free(path_directorio_bloque);
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
void guardar_en_bloque(char* path_bloque,char* dato){
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
}

int el_bloque_esta_lleno(int bloque){
	return tamanio_libre_del_bloque(bloque) == 0;
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

	int posicion_ultimo_bloque = tamanio_de_lista (lista_bloques) - 1;
	char* ultimo_bloque = string_duplicate(lista_bloques[posicion_ultimo_bloque]);
	int numero_ultimo_bloque = atoi(ultimo_bloque);

	free(ultimo_bloque);

	for(int i = 0; i < tamanio_de_lista(lista_bloques); i++){
		free(lista_bloques[i]);
	}
	free(lista_bloques);

	return numero_ultimo_bloque;
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


char* devolver_lista_de_bloques(char* path_particion){
	//pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
	t_config* particion = config_create(path_particion);
	char* bloques = string_duplicate(config_get_string_value(particion,"BLOCKS"));
	config_destroy(particion);
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);
	return bloques;
}



