#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include "socket.h"

int iniciar_servidor(char* puerto);
int esperar_cliente(int);
void *recibir_buffer(int *, int);
int recibir_operacion(int);
void *recibir_mensaje(int);

#endif /* SERVER_UTILS_H_ */
