#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <commons/log.h>
#include "instrucciones.h"
#include "socket.h"

t_log *logger;

int iniciar_servidor(void);
int esperar_cliente(int);
int recibir_operacion(int);
void *recibir_buffer(int*, int);
void recibir_mensaje(int);

int recibir_tamanio_en_memoria(void *buffer);

#endif /* SERVER_UTILS_H_ */
