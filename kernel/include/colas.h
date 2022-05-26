#ifndef COLAS_H_
#define COLAS_H_

#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdio.h>

#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/log.h>

#include <estructuras/pcb.h>
#include <sockets/server_utils.h>
#include "leer_config.h"

extern t_log *logger;

t_queue *new;
t_list *ready;

pthread_mutex_t mnew;
pthread_mutex_t mready;

sem_t grado_multip;
sem_t procesos;

void conectar_consola();

void proceso_new(int *socket_cliente);
void agregar_a_ready_fifo(t_pcb *pcb, char *algoritmo);
void agregar_a_ready_sjf(t_pcb *pcb, char *algoritmo);

#endif /* COLAS_H_ */
