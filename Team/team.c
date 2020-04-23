#include "team.h"


//Creacion del hilo


/*void* sumar();
void* restar();
pthread_t entrenadores[3];
*/
int i=0;


int main(void){

	t_queue* cola_de_entrenadores = queue_create();
	queue_push()

	cola_de_entrenadores = (cola*)malloc(sizeof(cola));
	cola_de_entrenadores -> inicio = NULL;

	queue_create(cola_de_entrenadores);
	queue_push()



	/*pthread_create(&(entrenadores[0]),NULL,sumar,NULL);

	pthread_create(&(entrenadores[1]),NULL,restar,NULL);


	pthread_join(entrenadores[0],NULL);
	pthread_join(entrenadores[1],NULL);

	printf("El valor de i es : %d \n ",i);
	*/



	return 0;
}

/*void* sumar(){



	while(i<10){
		i++;
	}
	return NULL;
}

void* restar(){

	int j=0;
	while(j<10){
		j++;
		i--;
	}
	return NULL;
}
*/

void agregar_entrenador_al_final_de_la_cola(pthread_t un_entrenador, cola *una_cola)
{
    nodo *aux;
    nodo * un_nodo;

    un_nodo = (nodo*)malloc(sizeof(nodo));
    un_nodo -> entrenador = un_entrenador;

    if(la_cola_esta_vacia(una_cola))
    {
    	llenar_nodo(un_nodo,una_cola);
    }
    else
    {

    	aux = una_cola -> inicio;

    	while(hay_un_nodo_siguiente(aux))
    	{
    		aux = aux -> ptr;
    	}
    	llenar_nodo(aux,una_cola);
    }

    free(un_nodo);

}

int la_cola_esta_vacia(cola *una_cola){
	if(una_cola -> inicio == NULL)
	{
		return 1;
	}
	else {
		return 0;
	}
}

void llenar_nodo(nodo *un_nodo, cola *una_cola){
	un_nodo -> ptr = NULL;
	una_cola -> inicio = un_nodo;
}

int hay_un_nodo_siguiente(nodo* un_nodo){
	if(un_nodo -> ptr != NULL){
		return 1;
	}
	else
	{
		return 0;
	}
}

pthread_t obtener_primer_entrenador(pthread_t un_entrenador, cola *una_cola) {

	if(la_cola_esta_vacia(una_cola))
	{
	    return NULL;
	}
	else
	{
		nodo *aux;
		aux = una_cola -> inicio;

		if(hay_un_nodo_siguiente(aux)){
			una_cola -> inicio -> ptr;
			return aux -> entrenador;
		}
		else
		{
			return aux -> entrenador;
		}



	}
}
