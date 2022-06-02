#include "../include/conexion_cpu.h"

void conectar_cpu(char* ip, char* puerto_dispatch, char* puerto_interrupt){

    int socket_dispatch;
    int socket_interrupt;

    socket_dispatch = crear_conexion(ip, puerto_dispatch);
    socket_interrupt = crear_conexion(ip, puerto_interrupt);

}
