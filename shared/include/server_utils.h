#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "socket.h"

int iniciar_servidor(void);
int esperar_cliente(int);
void *recibir_buffer(int *, int);
int recibir_operacion(int);

#endif /* SERVER_UTILS_H_ */
