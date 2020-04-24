#ifndef UTILS_H_
#define UTILS_H_

#include "config-reader.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/queue.h>
#include<readline/readline.h>

t_log* logger;
t_log* nuestro_log;

void conectar_broker(void);

#endif /* UTILS_H_ */
