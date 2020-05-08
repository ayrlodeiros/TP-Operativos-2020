#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>

#include<commons/log.h>
#include<commons/config.h>

t_log* logger;
t_log* mi_log;

typedef enum{
	GAME_BOY = 1,
	GAME_CARD = 2,
	TEAM = 3
}modulo_code;

#endif /* UTILS_H_ */
