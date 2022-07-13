#ifndef COLAS_H_
#define COLAS_H_


#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include <commons/log.h>
#include <commons/collections/queue.h>

#include "../../shared/include/sockets/server_utils.h"
#include "../../shared/include/estructuras/pcb.h"
#include "colas.h"
#include "conexion_memoria.h"



void conectar_consola(char *puerto);
void proceso_new(int *socket_cliente);

#endif /* COLAS_H_ */
