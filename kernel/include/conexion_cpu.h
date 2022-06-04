#ifndef CONEXION_CPU_H_
#define CONEXION_CPU_H_

#include "../../shared/include/sockets/client_utils.h"
#include "../../shared/include/sockets/server_utils.h"
#include "../../shared/include/estructuras/pcb.h"
#include "../../shared/include/estructuras/instrucciones.h"
#include <commons/log.h>

t_log* kernel_cpu_log;
extern t_log* logger;
int socket_dispatch;
int socket_interrupt;

void conectar_cpu(char* ip, char* puerto_dispatch, char* puerto_interrupt);

#endif