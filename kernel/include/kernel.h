#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdlib.h>

#include "conexion_consola.h"
#include "conexion_memoria.h"
#include "conexion_cpu.h"
#include "leer_config.h"
#include "colas.h"

t_log *logger;
t_config *config;
typedef struct{
    int milisegundos;
    t_pcb* pcb;
} t_pcb_con_milisegundos;

extern t_queue *queue_new;
extern t_queue *exec;
extern t_queue *suspendido;
extern t_queue *queue_exit;

extern sem_t mutex_new, mutex_suspendido, mutex_exit, mutex_memoria;;
extern sem_t procesos_en_new, procesos_en_exec, procesos_en_suspendido, procesos_en_exit;
extern sem_t disponible_para_exec;


void inicializar_colas_checkpoint();
void *thread_de_new();
void *thread_de_exec();
void *thread_de_suspendido();
void *thread_de_exit();

t_pcb_con_milisegundos recibir_proceso();
void ejecutar_proceso(t_pcb* pcb);

#endif /* KERNEL_H_ */
