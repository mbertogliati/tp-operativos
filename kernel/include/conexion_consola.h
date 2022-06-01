#ifndef COLAS_H_
#define COLAS_H_

#include <string.h>
#include <stdio.h>
#include <pthread.h>

#include <commons/log.h>

#include <sockets/server_utils.h>
#include <estructuras/pcb.h>
#include "colas.h"

extern t_log *logger;

void conectar_consola(char *puerto);
void proceso_new(int *socket_cliente);

#endif /* COLAS_H_ */
