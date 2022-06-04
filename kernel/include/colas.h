#ifndef COLAS_H_
#define COLAS_H_

#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include <commons/collections/list.h>
#include <commons/collections/queue.h>

#include "../../shared/include/sockets/client_utils.h"
#include "../../shared/include/estructuras/pcb.h"
#include "conexion_memoria.h"
#include "leer_config.h"

//t_queue *new;
t_list *ready;

pthread_mutex_t mnew;
pthread_mutex_t mready;

sem_t grado_multip;
sem_t procesos;


//Checkpoint
t_queue *queue_new;
t_queue *exec;
t_queue *suspendido;
t_queue *queue_exit;

sem_t mutex_new, mutex_suspendido, mutex_exit, mutex_memoria;;
sem_t procesos_en_new, procesos_en_exec, procesos_en_suspendido, procesos_en_exit;
sem_t disponible_para_exec;


void inicializar_colas();
void agregar_a_new(t_pcb *pcb);
void agregar_a_ready(t_pcb *pcb);

#endif /* COLAS_H_ */
