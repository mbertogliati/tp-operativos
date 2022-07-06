#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdlib.h>

#include "colas.h"
#include "conexion_consola.h"
#include "conexion_memoria.h"
#include "conexion_cpu.h"
#include "leer_config.h"
#include <commons/log.h>

t_log *logger;
t_config *config;
typedef struct{
    int milisegundos;
    t_pcb* pcb;
} t_pcb_con_milisegundos;

t_log *log_kernel;


#endif /* KERNEL_H_ */
