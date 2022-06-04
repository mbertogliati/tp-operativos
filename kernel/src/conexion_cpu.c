#include "../include/conexion_cpu.h"

void conectar_cpu(char* ip, char* puerto_dispatch, char* puerto_interrupt){

    //logger = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);
    printf("Iniciando conexion con el Cpu\n");

    while(socket_interrupt = crear_conexion(ip, puerto_interrupt) <= 0){
        printf("No se ha podido establecer la comunicacion con el Interrupt\n");
        sleep(5);
    }
    printf("Conexion con Interrupt Exitosa\n");
    

    while(socket_dispatch = crear_conexion(ip, puerto_dispatch) <= 0){
        printf("No se ha podido establecer la comunicacion con el Dispatch\n");
        sleep(5);
    }
    printf("Conexion con Dispatcher Exitosa\n");

}
