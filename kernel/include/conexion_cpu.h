#ifndef CONEXION_CPU_H_
#define CONEXION_CPU_H_

#include "../../shared/include/sockets/client_utils.h"
#include "../../shared/include/sockets/server_utils.h"
#include "../../shared/include/estructuras/pcb.h"
#include "../../shared/include/estructuras/instrucciones.h"

void conectar_cpu(char* ip, char* puerto_dispatch, char* puerto_interrupt);

#endif