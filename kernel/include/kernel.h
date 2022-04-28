#ifndef KERNEL_H_
#define KERNEL_H_
#include "../../shared/include/server_utils.h"

typedef struct{
    int memory_size;
    t_list* lista_de_instrucciones;
}t_paquete_consola;


t_paquete_consola *recibir_paquete_instrucciones_consola(int socket_cliente);

#endif
