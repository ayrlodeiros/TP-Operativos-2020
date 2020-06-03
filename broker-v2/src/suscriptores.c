#include "suscriptores.h"


void switch_cola(int cod_op, int socket_cliente, modulo_code modulo){
	int cola;
	recv(socket_cliente,&cola,sizeof(int),MSG_WAITALL);
	switch (cola){
			case GET:
				switch_operacion(cod_op,get_mq,socket_cliente,modulo);
				break;
			case LOCALIZED:
				switch_operacion(cod_op,localized_mq,socket_cliente,modulo);
				break;
			case CATCH:
				switch_operacion(cod_op,catch_mq,socket_cliente,modulo);
				break;
			case CAUGHT:
				switch_operacion(cod_op,caught_mq,socket_cliente,modulo);
				break;
			case NEW:
				switch_operacion(cod_op,new_mq,socket_cliente,modulo);
				break;
			case APPEARED:
				switch_operacion(cod_op,appeared_mq,socket_cliente,modulo);
				break;
			default:
				log_info(mi_log,"HUbo un error al tratar de recibir el mensaje\n");
				pthread_exit(NULL);
	}
}

void switch_operacion (op_code operacion, t_mq cola,int conexion, modulo_code modulo){
	suscriptor_t* suscriptor;
	switch(operacion){
	case MENSAJE:
		recibir_y_guardar_mensaje(conexion,cola);
		break;
	case SUSCRIPCION:
		suscriptor = crear_suscriptor(conexion,modulo);
		agregar_suscriptor_cola(cola,suscriptor);
		break;
	}
}

/*
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



void switch_tipo_msj(int op_code){
	switch (op_code){
	case MENSAJE:
		switch_suscipcion();
		break;
	case SUSCRIPCION:
		break;
	}

}
*/
suscriptor_t* crear_suscriptor(int conexion_suscriptor, modulo_code codigo_suscriptor){
	suscriptor_t* suscriptor = malloc(sizeof(suscriptor));
	suscriptor->conexion = conexion_suscriptor;
	suscriptor->modulo = codigo_suscriptor;
	return suscriptor;
}
