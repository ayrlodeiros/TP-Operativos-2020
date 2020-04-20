#include "team.h"


//Creacion del hilo


void* contar_hasta_100();
void* saludar();
pthread_t entrenadores[3];

int main(void){


	pthread_create(&(entrenadores[0]),NULL,contar_hasta_100,NULL);

	pthread_create(&(entrenadores[1]),NULL,saludar,NULL);


	pthread_join(entrenadores[0],NULL);
	pthread_join(entrenadores[1],NULL);


	return 0;
}

void* contar_hasta_100(){

	int i=0;

	while(i<100){
		printf("El valor de i es : %d",i);
		i++;
	}
	return NULL;
}

void* saludar(){
	int i = 0;

	while(i<100){
		printf("Hola :%d",i);
		i++;
	}
	return NULL;
}

