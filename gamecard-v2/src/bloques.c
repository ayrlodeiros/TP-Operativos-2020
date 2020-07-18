#include "bloques.h"

<<<<<<< HEAD
/*
//guarda la data en el archivo del path
void escribir_bloque(char* path_config,char* dato){

	char* path_bloque;
	char* path_directorio_bloque = devolver_path_directorio("/Blocks");
	int tamanio_disponible_del_ultimo_bloque = tamanio_libre_del_ultimo_bloque(path_config);
	log_info(nuestro_log,"Tamanio Disponible:%d",tamanio_disponible_del_ultimo_bloque);


	t_config* config_pokemon = config_create(path_config);

	if(list_is_empty(obtener_blocks_archivo_metadata_pokemon("Pikachu"))){
		guardar_en_bloque(obtener_primer_bloque_libre("kgaas"),posicion,cantidad);
	}

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
*/
=======
>>>>>>> develop
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
<<<<<<< HEAD
		//bitarray_set_bit(bitmap,i);
=======
>>>>>>> develop
	}
}

void agregar_dato_al_bloque (char* path_nombre_metadata,char* dato_a_escribir){
	int ultimo_bloque = devolver_ultimo_bloque(path_nombre_metadata);
<<<<<<< HEAD
	bitarray_set_bit(bitmap,ultimo_bloque);
	char* string_dato = string_itoa(devolver_ultimo_bloque(path_nombre_metadata));
	char* path_bloque = devolver_path_dato(string_dato);
=======
	log_info(logger,"se escribio el bloque %d perteneciente al metadata %s  con el dato %s",ultimo_bloque,path_nombre_metadata,dato_a_escribir);
	bitarray_set_bit(bitmap,ultimo_bloque);
	char* string_dato = string_itoa(devolver_ultimo_bloque(path_nombre_metadata));
	char* path_bloque = devolver_path_dato(string_dato);
	free(string_dato);
>>>>>>> develop
	guardar_en_bloque(path_bloque,dato_a_escribir);
	actualizar_tamanio_bloque(path_nombre_metadata);
	free(path_bloque);
}

void escribir_bloque_v2(char* path_nombre_metadata,char* dato_a_escribir){

	int tamanio_disponible_del_ultimo_bloque = tamanio_libre_del_ultimo_bloque(path_nombre_metadata);

	int tamanio_dato = strlen(dato_a_escribir);

	char* lista_de_bloques_string = devolver_lista_de_bloques(path_nombre_metadata);
	char** lista_de_bloques = string_get_string_as_array(lista_de_bloques_string);
	free(lista_de_bloques_string);
	t_list* bloques_del_metadata = crear_t_list(lista_de_bloques);
<<<<<<< HEAD

	log_info(nuestro_log,"Tamaño lista : %d",list_size(bloques_del_metadata));
	if(list_is_empty(bloques_del_metadata)){
		agregar_bloques_al_metadata(path_nombre_metadata,tamanio_dato,obtener_block_size());
	}
	log_info(nuestro_log,"ENTROO 0");
	list_destroy(bloques_del_metadata);
	free(lista_de_bloques);
	//free(lista_de_bloques_string);

	//Si el dato no entra en el bloque
	if(tamanio_dato > tamanio_disponible_del_ultimo_bloque - 1){
		log_info(nuestro_log,"ENTROO 1");
		agregar_bloques_al_metadata(path_nombre_metadata,tamanio_dato,tamanio_disponible_del_ultimo_bloque);
		int bloque_a_escribir = obtener_primer_bloque_libre(path_nombre_metadata);
		char* path_bloque_a_escribir = devolver_path_dato(string_itoa(bloque_a_escribir));
		char* a_escribir = string_new();
		a_escribir = string_substring(dato_a_escribir, 0, tamanio_disponible_del_ultimo_bloque);
		guardar_en_bloque(path_bloque_a_escribir,a_escribir);
		log_info(nuestro_log,"ENTROO 2");

		actualizar_tamanio_bloque(path_nombre_metadata);
		char* dato_a_escribir_restante = string_new();
		dato_a_escribir_restante = string_substring_from(dato_a_escribir, strlen(a_escribir));
		int tamanio_dato_a_escribir_restante = strlen(dato_a_escribir_restante);
		log_info(nuestro_log,"ENTROO 3");
		if(obtener_primer_bloque_libre(path_nombre_metadata) != -1){
			int bloque_nuevo_a_escribir = obtener_primer_bloque_libre(path_nombre_metadata);
			char* path_bloque_nuevo_a_escribir = devolver_path_dato(string_itoa(bloque_nuevo_a_escribir));

			guardar_en_bloque(path_bloque_nuevo_a_escribir,dato_a_escribir_restante);

			actualizar_tamanio_bloque(path_nombre_metadata);
			free(path_bloque_nuevo_a_escribir);
			log_info(nuestro_log,"ENTROO 4");
		}

=======
	list_destroy(bloques_del_metadata);
	for(int i = 0; i < tamanio_de_lista(lista_de_bloques); i++){
		free(lista_de_bloques[i]);
	}
	free(lista_de_bloques);

	//Si el dato no entra en el bloque
	// DATO = 111111111-55555555 = 999999
	// TAM = 8 => 1111111"\n" 11-55555"\n"
	if(tamanio_dato > tamanio_disponible_del_ultimo_bloque){
		agregar_bloques_al_metadata(path_nombre_metadata,tamanio_dato,tamanio_disponible_del_ultimo_bloque);
		int bloque_a_escribir = obtener_primer_bloque_libre(path_nombre_metadata);
		char* string_bloque_a_escribir = string_itoa(bloque_a_escribir);
		char* path_bloque_a_escribir = devolver_path_dato(string_bloque_a_escribir);
		char* a_escribir = "";
		a_escribir = string_substring(dato_a_escribir, 0, tamanio_disponible_del_ultimo_bloque);
		guardar_en_bloque(path_bloque_a_escribir,a_escribir);

		actualizar_tamanio_bloque(path_nombre_metadata);
		char* dato_a_escribir_restante = "";
		dato_a_escribir_restante = string_substring_from(dato_a_escribir, strlen(a_escribir));

		escribir_bloque_sin_asignar(path_nombre_metadata,dato_a_escribir_restante);
>>>>>>> develop

		free(a_escribir);
		free(dato_a_escribir_restante);
		free(path_bloque_a_escribir);
<<<<<<< HEAD
		log_info(nuestro_log,"ENTROO 5");
=======
		free(string_bloque_a_escribir);
>>>>>>> develop
	}
	//en el ultimo bloque hay espacio suficiente para guardar la info completa
	else{
		agregar_dato_al_bloque(path_nombre_metadata,dato_a_escribir);
	}
<<<<<<< HEAD
	log_info(nuestro_log,"ENTROO 6");
}

//trae todas los inserts de esa url, que es la particion.bin o el .tmp
t_list* leer_datos(char* path_metadata_config){
	FILE *archivo;
	int tamanio_archivo;
	char* lista_de_bloques_string = devolver_lista_de_bloques(path_metadata_config);
	char** lista_de_bloques = string_get_string_as_array(lista_de_bloques_string);
	free(lista_de_bloques_string);
	int size = tamanio_de_lista(lista_de_bloques); // tamano de array de bloques
	char* dato = string_new();
	char* path_bloque; // url de cada block particular
	char* pivot;
	struct stat st;
	for(int i = 0; i<size; i++)
	{
		path_bloque = devolver_path_dato(string_itoa(i));
		stat(path_bloque,&st);
		tamanio_archivo = st.st_size;

		pivot = malloc(tamanio_archivo+1);
		archivo = fopen(path_bloque,"r");
		fread(pivot,tamanio_archivo,1,archivo);
		fclose(archivo);
		pivot[tamanio_archivo] = '\0';
		if(strcmp(pivot,"&")) //si no es igual a "&" lo agrego a la lista de inserts
			string_append(&dato,pivot);

		free(lista_de_bloques[i]);
		free(pivot);
		free(path_bloque);
	}
	t_list* lista_de_posiciones = list_create();

	agregar_datos_a_la_lista(dato,lista_de_posiciones,path_metadata_config);
	 //parsea el char *inserts por \n y los mete en la lista
	free(dato);
	free(lista_de_bloques);
	return lista_de_posiciones;
}

int cantidad_en_posicion(t_list* lista_de_posiciones,char* posicion_a_buscar){

	int posicion_encontrada = posicion_en_la_lista_de_posiciones_pokemon_a_buscar(lista_de_posiciones,posicion_a_buscar);
	char* aux = string_new();
	char* pivot_aux = list_get(lista_de_posiciones,posicion_encontrada);
	string_append(&aux,pivot_aux);
	char** aux_partido = string_split(aux,"=");
	char* cantidad_encontrada_string = aux_partido[1];
	int cantidad_encontrada = atoi(cantidad_encontrada_string);
	free(aux);
	free(aux_partido);

	return cantidad_encontrada;

=======
}

void escribir_bloque_sin_asignar(char* path_nombre_metadata,char* dato_a_escribir_restante){

	int bloque_a_escribir = obtener_primer_bloque_libre(path_nombre_metadata);
	char* string_bloque_a_escribir = string_itoa(bloque_a_escribir);
	char* path_bloque_a_escribir = devolver_path_dato(string_bloque_a_escribir);
	free(string_bloque_a_escribir);
	char* a_escribir = "";
	a_escribir = string_substring(dato_a_escribir_restante, 0, tamanio_libre_del_bloque(bloque_a_escribir));
	guardar_en_bloque(path_bloque_a_escribir,a_escribir);
	free(a_escribir);
	actualizar_tamanio_bloque(path_nombre_metadata);
	free(path_bloque_a_escribir);

	if(string_length(dato_a_escribir_restante) > obtener_block_size()){
		char* a_escribir_restante = string_new();
		a_escribir_restante = string_substring_from(dato_a_escribir_restante, obtener_block_size());
		free(dato_a_escribir_restante);
		escribir_bloque_sin_asignar(path_nombre_metadata,a_escribir_restante);
		free(a_escribir_restante);
	}
>>>>>>> develop

}

int se_encuentra_la_posicion_en_la_lista(t_list* lista_de_posiciones,char* posicion_a_buscar){
	char* posicion;
	if(list_is_empty(lista_de_posiciones)){
		return 0;
	}

	for(int i = 0; i< list_size(lista_de_posiciones);i++){
		posicion = list_get(lista_de_posiciones,i);
		if(string_equals_ignore_case(posicion_a_buscar,posicion)){
			return 1;
		}
	}
	return 0;
}

int se_encuentra_la_posicion_en_la_lista_de_posiciones_pokemons(t_list* lista_de_posiciones,char* posicion_a_buscar){
	char* posicion;
	if(list_is_empty(lista_de_posiciones)){
		return 0;
	}

	for(int i = 0; i< list_size(lista_de_posiciones);i++){
		char* aux = string_new();
		char* pivot_aux = list_get(lista_de_posiciones,i);
		string_append(&aux,pivot_aux);
		char** aux_partido = string_split(aux,"=");
		posicion = aux_partido[0];
		if(string_equals_ignore_case(posicion_a_buscar,posicion)){
<<<<<<< HEAD
			return 1;
		}

=======
			free(posicion);
			free(aux_partido[1]);
			free(aux_partido);
			free(aux);
			return 1;
		}
		free(posicion);
		free(aux_partido[1]);
>>>>>>> develop
		free(aux_partido);
		free(aux);
	}
	return 0;
}

int posicion_en_la_lista_de_la_posicion_a_buscar(t_list* lista_de_posiciones,char* posicion_a_buscar){
	char* posicion;
	for(int i = 0; i< list_size(lista_de_posiciones);i++){
		posicion = list_get(lista_de_posiciones,i);
		if(string_equals_ignore_case(posicion_a_buscar,posicion)){
<<<<<<< HEAD
			return i;
		}
=======
			free(posicion);
			return i;
		}
		free(posicion);
>>>>>>> develop
	}
	return 0;
}

int posicion_en_la_lista_de_posiciones_pokemon_a_buscar(t_list* lista_de_posiciones,char* posicion_a_buscar){
	char* posicion;
	for(int i = 0; i< list_size(lista_de_posiciones);i++){
<<<<<<< HEAD
		char* aux = string_new();
		char* pivot_aux = list_get(lista_de_posiciones,i);
		string_append(&aux,pivot_aux);
		char** aux_partido = string_split(aux,"=");
		posicion = aux_partido[0];
		if(string_equals_ignore_case(posicion_a_buscar,posicion)){
			return i;
		}

		free(aux_partido);
		free(aux);
=======
		posicion = obtener_posicion_del_dato(list_get(lista_de_posiciones,i));
		if(string_equals_ignore_case(posicion_a_buscar,posicion)){
			free(posicion);
			return i;
		}
		free(posicion);
>>>>>>> develop
	}
	return 0;
}


void agregar_datos_a_la_lista(char *dato,t_list* lista_de_posiciones,char* path_nombre_metadata){

	char** lista_de_datos = string_split(dato,"\n");
	char* pivot;
	for(int i =0; i<tamanio_de_lista(lista_de_datos); i++)
	{
		pivot = string_duplicate(lista_de_datos[i]);
		char** pivot_partido = string_split(pivot,"=");
		char* posicion = pivot_partido[0];
		char* cantidad_string = pivot_partido[1];
		int cantidad = atoi(cantidad_string);
		char* dato_a_guardar;
<<<<<<< HEAD
		/*
		int posicion_encontrada = posicion_en_la_lista_de_posiciones_pokemon_a_buscar(lista_de_posiciones,posicion);
		if(se_encuentra_la_posicion_en_la_lista_de_posiciones_pokemons(lista_de_posiciones,posicion)){
			log_info(nuestro_log,"ENTRE AL IF");
			char* aux = string_new();
			char* pivot_aux = list_remove(lista_de_posiciones,posicion_encontrada);
			string_append(&aux,pivot_aux);
			char** aux_partido = string_split(aux,"=");
			int cantidad_encontrada = atoi(aux[1]);
			int nueva_cantidad = cantidad + cantidad_encontrada;

			dato_a_guardar = armar_dato_bloque(posicion,string_itoa(nueva_cantidad));
			log_info(nuestro_log,"DATO A GUARDAR : %s",dato_a_guardar);
			list_add(lista_de_posiciones,dato_a_guardar);
			free(aux);
			free(aux_partido);

		}else{*/
			dato_a_guardar = armar_dato_bloque(posicion,cantidad);
			list_add(lista_de_posiciones,dato_a_guardar);
		//}

		free(lista_de_datos[i]);
		//free(pivot_partido);
=======

		dato_a_guardar = armar_dato_bloque(posicion,cantidad);
		list_add(lista_de_posiciones,dato_a_guardar);

		free(posicion);
		free(cantidad_string);
		free(lista_de_datos[i]);
		free(pivot_partido);
>>>>>>> develop
		free(pivot);
	}
	free(lista_de_datos);

<<<<<<< HEAD
	/*
	 * if(dictionary_has_key(objetivo_global, pokemon_objetivo)) {
		dictionary_put(objetivo_global, pokemon_objetivo, (void*) (dictionary_get(objetivo_global, pokemon_objetivo) + 1));
	}
	//Si el pokemon no existia lo agrego al diccionario con un valor de 1
	else {
		int valor_inicial = 1;
		dictionary_put(objetivo_global, pokemon_objetivo, valor_inicial);
	 */
}
/*
void reemplazar_cantidad_en_la_posicion_en_el_archivo(char* path_metadata_config,char* posicion_a_buscar,char* nuevo_cantidad){
	FILE *archivo;
		int tamanio_archivo;
		char* lista_de_bloques_string = devolver_lista_de_bloques(path_metadata_config);
		char** lista_de_bloques = string_get_string_as_array(lista_de_bloques_string);
		free(lista_de_bloques_string);
		int size = tamanio_de_lista(lista_de_bloques); // tamano de array de bloques

		char* dato = string_new();
		char* path_bloque; // url de cada block particular
		char* pivot;
		struct stat st;
		for(int i = 0; i<size; i++)
		{
			path_bloque = devolver_path_dato(string_itoa(i));
			stat(path_bloque,&st);
			tamanio_archivo = st.st_size;

			pivot = malloc(tamanio_archivo+1);

			archivo = fopen(path_bloque,"r");
			fread(pivot,tamanio_archivo,1,archivo);
			fclose(archivo);
			pivot[tamanio_archivo] = '\0';

			if(strcmp(pivot,"&")) //si no es igual a "&" lo agrego a la lista de inserts
				string_append(&dato,pivot);

			free(lista_de_bloques[i]);
			free(pivot);
			free(path_bloque);
		}

		las_posiciones_son_iguales(dato,posicion_a_buscar,nuevo_cantidad);
		 //parsea el char *inserts por \n y los mete en la lista
		free(dato);
		free(lista_de_bloques);

}
*/

void borrar_posicion(t_list* lista_de_posiciones, char* dato_a_escribir){
=======
}

void borrar_posicion(t_list* lista_de_posiciones, char* dato_a_escribir){
	char* posicion_nueva = obtener_posicion_del_dato(dato_a_escribir);
	int indice = posicion_en_la_lista_de_posiciones_pokemon_a_buscar(lista_de_posiciones,posicion_nueva);
	free(list_remove(lista_de_posiciones,indice));
	free(posicion_nueva);
}

char* obtener_posicion_del_dato(char* dato_a_escribir){
	char* aux;
	char** aux_partido;
	aux = string_new();
	string_append(&aux,dato_a_escribir);
	aux_partido = string_split(aux,"=");
	char* cantidad = aux_partido[1];
	free(cantidad);
	char* posicion = aux_partido[0];
	free(aux);
	free(aux_partido);
	return posicion;
}

int obtener_posicion_x_del_dato(char* dato_a_escribir){

	char* aux;
	char* posicion = obtener_posicion_del_dato(dato_a_escribir);
	char** posicion_partida;
	aux = string_new();
	string_append(&aux,posicion);
	posicion_partida = string_split(aux,"-");
	int posicion_en_x = atoi(posicion_partida[0]);
	free(aux);
	free(posicion_partida[0]);
	free(posicion_partida[1]);
	free(posicion_partida);
	free(posicion);

	return posicion_en_x;
}

int obtener_posicion_y_del_dato(char* dato_a_escribir){

	char* aux;
	char* posicion = obtener_posicion_del_dato(dato_a_escribir);
	char** posicion_partida;
	aux = string_new();
	string_append(&aux,posicion);
	posicion_partida = string_split(aux,"-");
	int posicion_y = atoi(posicion_partida[1]);
	free(aux);
	free(posicion_partida[1]);
	free(posicion_partida[0]);
	free(posicion_partida);
	free(posicion);
	return posicion_y;
}

int obtener_cantidad_del_dato(char* dato_a_escribir){
>>>>>>> develop
	char* aux;
	char** aux_partido;
	aux = string_new();
	string_append(&aux,dato_a_escribir);
	aux_partido = string_split(aux,"=");
<<<<<<< HEAD
	char* posicion_nueva = aux_partido[0];
	free(aux);
	free(aux_partido);
	int indice = posicion_en_la_lista_de_posiciones_pokemon_a_buscar(lista_de_posiciones,posicion_nueva);
	free(list_remove(lista_de_posiciones,indice));
	return;
}

void reescribir_bloques(char* path_nombre_metadata,char* dato_a_escribir){
=======
	int cantidad = atoi(aux_partido[1]);
	free(aux);
	free(aux_partido[0]);
	free(aux_partido[1]);
	free(aux_partido);
	return cantidad;
}

void reescribir_bloques(char* path_nombre_metadata,char* dato_a_escribir){

>>>>>>> develop
	char* lista_de_bloques_string = devolver_lista_de_bloques(path_nombre_metadata);
	char** lista_de_bloques = string_get_string_as_array(lista_de_bloques_string);
	free(lista_de_bloques_string);
	int size = tamanio_de_lista(lista_de_bloques);
	t_list* bloques = crear_t_list(lista_de_bloques);
	t_list* lista_de_posiciones = leer_datos(path_nombre_metadata);
	borrar_posicion(lista_de_posiciones,dato_a_escribir);
<<<<<<< HEAD
	list_add(lista_de_posiciones,dato_a_escribir);


	for(int i = 0; i < size; i++){
		limpiar_bloque(i);
	}
	sacar_bloques_metadata_pokemon(path_nombre_metadata);
	log_info(nuestro_log,"Prueba 3");
	for(int j = 0 ; j < list_size(lista_de_posiciones); j++){
		escribir_bloque_v2(path_nombre_metadata,list_get(lista_de_posiciones,j));
		log_info(nuestro_log,"ENTROO 7");
	}
	log_info(nuestro_log,"Prueba 4");
	//free(path_nombre_metadata);

}

/*
void las_posiciones_son_iguales(char *dato,char* posicion_a_buscar,char* nuevo_cantidad){

	char** lista_de_datos = string_split(dato,"\n");
	char* pivot;
	for(int i =0; i<tamanio_de_lista(lista_de_datos); i++)
	{

		pivot = string_duplicate(lista_de_datos[i]);
		char** pivot_partido = string_split(pivot,"=");
		char* posicion = pivot_partido[0];
		char* cantidad = pivot_partido[1];

		log_info(nuestro_log,"Posicion a buscar : %s",posicion_a_buscar);
		log_info(nuestro_log,"Posicion : %s",posicion);

		if(string_equals_ignore_case(posicion,posicion_a_buscar)){
			int largo_cantidad = strlen(cantidad);
			int largo_nueva_cantidad = strlen(nuevo_cantidad);

			if(largo_cantidad == largo_nueva_cantidad){

			}


			log_info(nuestro_log,"Se encontro la posicion : %s", posicion_a_buscar);
			free(lista_de_datos[i]);
			return;
		}
	}
	free(lista_de_datos);
	log_info(nuestro_log,"NO se encontro la posicion : %s", posicion_a_buscar);

}
*/

=======

	if(obtener_cantidad_del_dato(dato_a_escribir) != 0){
		list_add(lista_de_posiciones,dato_a_escribir);
	}
	for(int i = 0; i < size; i++){
		int bloque_a_limpiar = atoi(lista_de_bloques[i]);
		limpiar_bloque(bloque_a_limpiar);
		free(lista_de_bloques[i]);
	}
	resetear_bloques_metadata_pokemon(path_nombre_metadata);
	for(int j = 0 ; j < list_size(lista_de_posiciones); j++){
		escribir_bloque_v2(path_nombre_metadata,list_get(lista_de_posiciones,j));
	}
	free(lista_de_bloques);
	list_destroy(bloques);
	list_destroy_and_destroy_elements(lista_de_posiciones,free);

}
>>>>>>> develop

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
<<<<<<< HEAD
	char* path_bloque = devolver_path_dato(string_itoa(bloque));
=======
	char* string_bloque = string_itoa(bloque);
	char* path_bloque = devolver_path_dato(string_bloque);
>>>>>>> develop

	FILE *archivo = fopen(path_bloque,"w"); //limpio el archivo
	fclose(archivo);
	liberar_bloque(bloque); //libero en bitarray

<<<<<<< HEAD
	free(path_bloque);
=======
	log_info(logger,"se limpio el bloque %d",bloque);
	free(path_bloque);
	free(string_bloque);
>>>>>>> develop
}


int se_creo_el_bloque(){
	char* path_bloque = devolver_path_directorio("/Blocks");
	string_append(&path_bloque,"0.bin");

	if(access(path_bloque,F_OK) != -1){
		free(path_bloque);
		return true;
	}
	else {
		free(path_bloque);
		return false;
	}
}

void crear_bloque(){ //ANDA BIEN
	char* path_bloque;
<<<<<<< HEAD

	for(int i = 0; i < obtener_blocks(); i++){
		path_bloque = devolver_path_dato(string_itoa(i));
=======
	char* string_i;
	for(int i = 0; i < obtener_blocks(); i++){
		string_i = string_itoa(i);
		path_bloque = devolver_path_dato(string_i);
		free(string_i);
>>>>>>> develop
		FILE* archivo = fopen(path_bloque,"w");
		fclose(archivo);
		free(path_bloque);
	}

}

void modificar_tamanio_bloque(char* path_bloque,int tamanio){
<<<<<<< HEAD
	//pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
=======
	pthread_mutex_lock(&mutex_modificar_bloque);
>>>>>>> develop
	t_config* config = config_create(path_bloque);
	char* tamanio_string = string_itoa(tamanio);
	config_set_value(config,"SIZE",tamanio_string);
	config_save(config);
	config_destroy(config);
	free(tamanio_string);
<<<<<<< HEAD
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);
=======
	pthread_mutex_unlock(&mutex_modificar_bloque);
>>>>>>> develop
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
void guardar_en_bloque(char* path_bloque, char* dato){

<<<<<<< HEAD
	/*
	char* path_bloque = devolver_path_dato(string_itoa(bloque));
	t_config* archivo_pokemon = config_create(path_bloque);
	config_set_value(archivo_pokemon,key_a_guardar,nueva_cantidad);
	config_save(archivo_pokemon);
	bitarray_set_bit(bitmap,bloque);
	*/
=======
>>>>>>> develop
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
<<<<<<< HEAD
=======
	log_info(logger,"se guardo el dato %s en el path del bloque %s",dato,path_bloque);
>>>>>>> develop
	txt_close_file(archivo2);
	free(pivot);

}

int tamanio_libre_real(int bloque){
	char* path_bloque = devolver_path_dato(string_itoa(bloque));

<<<<<<< HEAD
		struct stat st;
		stat(path_bloque,&st);

		int tamanio_actual = st.st_size;

		free(path_bloque);
		return (obtener_block_size() - tamanio_actual);
=======
	struct stat st;
	stat(path_bloque,&st);

	int tamanio_actual = st.st_size;

	free(path_bloque);
	return (obtener_block_size() - tamanio_actual);
>>>>>>> develop

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

<<<<<<< HEAD
		free(path);

		return (obtener_block_size() - tamanio_actual);
=======
	free(path);
	free(bloque_aux);
	return (obtener_block_size() - tamanio_actual);
>>>>>>> develop
}

//al archivo le agrego un nuevo bloque a la lista de bloques
void agregar_bloque(char* path_bloque){

	int nuevo_bloque = obtener_nuevo_bloque_libre();
<<<<<<< HEAD
=======
	log_info(logger,"se agrego el bloque %d al metadata %s",nuevo_bloque,path_bloque);
	bitarray_set_bit(bitmap,nuevo_bloque);
	log_info(logger,"se cambio el bit %d del bitmap a 1",nuevo_bloque);
>>>>>>> develop
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
<<<<<<< HEAD
	//pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
=======
	pthread_mutex_lock(&mutex_modificar_bloque);
>>>>>>> develop
	t_config* particion = config_create(path_particion);
	config_set_value(particion,"BLOCKS",lista_bloques);
	config_save(particion);
	config_destroy(particion);
<<<<<<< HEAD
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);
=======
	pthread_mutex_unlock(&mutex_modificar_bloque);
	log_info(logger,"se cambio los blocks del path %s por los siguientes bloques",path_particion,lista_bloques);
>>>>>>> develop

}

//no deberia salir del for, osea tiene que tener siempre asignado un bloque libre al menos
//-1 si sale del for, pero nodeberia pasar
//te devuelve el primer bloque libre de la tabla
<<<<<<< HEAD
int obtener_primer_bloque_libre(char* path_bloque){
	char* lista_bloques_string = devolver_lista_de_bloques(path_bloque);
=======
int obtener_primer_bloque_libre(char* path_nombre_metadata){
	char* lista_bloques_string = devolver_lista_de_bloques(path_nombre_metadata);
>>>>>>> develop
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
<<<<<<< HEAD
	/*
	for(int i= 0; i< obtener_blocks();i++){
		if(!bitarray_test_bit(bitmap,i)){
			return i;
		}
		log_error(nuestro_log,"no hay bloques libres");
		return -1;
	}*/
=======

>>>>>>> develop
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
<<<<<<< HEAD
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
=======

	int posicion_ultimo_bloque = tamanio_de_lista (lista_bloques) - 1;
	char* ultimo_bloque = string_duplicate(lista_bloques[posicion_ultimo_bloque]);
	int numero_ultimo_bloque = strtol(ultimo_bloque,NULL,10);
	free(ultimo_bloque);
	for(int i = 0; i < tamanio_de_lista(lista_bloques); i++){
		free(lista_bloques[i]);
	}
	free(lista_bloques);
	return numero_ultimo_bloque;
>>>>>>> develop
}


char* devolver_lista_de_bloques(char* path_files_config){
<<<<<<< HEAD
	//pthread_mutex_lock(&MUTEX_ELSOLUCIONADOR);
	t_config* config_file = config_create(path_files_config);
	char* bloques = string_duplicate(config_get_string_value(config_file,"BLOCKS"));
	config_destroy(config_file);
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONADOR);
=======
	pthread_mutex_lock(&mutex_modificar_bloque);
	t_config* config_file = config_create(path_files_config);
	char* bloques = string_duplicate(config_get_string_value(config_file,"BLOCKS"));
	config_destroy(config_file);
	pthread_mutex_unlock(&mutex_modificar_bloque);
>>>>>>> develop
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

<<<<<<< HEAD
=======
//Se agrega del bitmap.c
//libera el bloque
void liberar_bloque(int bloque){
	pthread_mutex_lock(&Mutex_Bitmap);
	bitarray_clean_bit(bitmap,bloque);
	flag_bloques_libres = 1;
	pthread_mutex_unlock(&Mutex_Bitmap);
}

>>>>>>> develop
