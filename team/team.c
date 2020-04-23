#include "team.h"


//Creacion del hilo


/*void* sumar();
void* restar();
pthread_t entrenadores[3];
*/
int i=0;


int main(void){

	t_queue* cola_de_entrenadores = queue_create();
	//queue_push();

	printf("Hola");
	asd();

	/*pthread_create(&(entrenadores[0]),NULL,sumar,NULL);

	pthread_create(&(entrenadores[1]),NULL,restar,NULL);


	pthread_join(entrenadores[0],NULL);
	pthread_join(entrenadores[1],NULL);

	printf("El valor de i es : %d \n ",i);
	*/

	return 0;
}
