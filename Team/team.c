#include "team.h"


//Creacion del hilo


void* sumar();
void* restar();
pthread_t entrenadores[3];
int i=0;

int main(void){


	pthread_create(&(entrenadores[0]),NULL,sumar,NULL);

	pthread_create(&(entrenadores[1]),NULL,restar,NULL);


	pthread_join(entrenadores[0],NULL);
	pthread_join(entrenadores[1],NULL);

	printf("El valor de i es : %d \n ",i);

	return 0;
}

void* sumar(){



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

