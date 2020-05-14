#include "suscriptores.h"

void switch_mensaje(int codigo_cola,int conexion){
	switch  (codigo_cola){
		case GET:
			recibir_y_guardar_mensaje(conexion,get_mq);
			break;
		case LOCALIZED:
			recibir_y_guardar_mensaje(conexion,get_mq);
			break;
		case CATCH:
			recibir_y_guardar_mensaje(conexion,catch_mq);
			break;
		case CAUGHT:
			recibir_y_guardar_mensaje(conexion,caught_mq);
			break;
		case NEW:
			recibir_y_guardar_mensaje(conexion,new_mq);
			break;
		case APPEARED:
			recibir_y_guardar_mensaje(conexion,appeared_mq);
			break;

	}
}
void switch_suscripcion(int codigo_cola, int conexion,modulo_code modulo_id){
	suscriptor_t* suscriptor = crear_suscriptor(conexion,modulo_id);
	switch  (codigo_cola){
		case GET:
			agregar_suscriptor_cola(get_mq,suscriptor);
			break;
		case LOCALIZED:
			agregar_suscriptor_cola(localized_mq,suscriptor);
			break;
		case CATCH:
			agregar_suscriptor_cola(catch_mq,suscriptor);
			break;
		case CAUGHT:
			agregar_suscriptor_cola(caught_mq,suscriptor);
			break;
		case NEW:
			agregar_suscriptor_cola(new_mq,suscriptor);
			break;
		case APPEARED:
			agregar_suscriptor_cola(appeared_mq,suscriptor);
			break;

	}
}
suscriptor_t* crear_suscriptor(int conexion_suscriptor, modulo_code codigo_suscriptor){
	suscriptor_t* suscriptor = malloc(sizeof(suscriptor));
	suscriptor->conexion = conexion_suscriptor;
	suscriptor->modulo = codigo_suscriptor;
	return suscriptor;
}
