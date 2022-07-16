#include "../include/conexion_cpu.h"

void conectar_cpu(char* ip, char* puerto_dispatch, char* puerto_interrupt){

    //logger = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);
    log_protegido("Iniciando conexion con el Cpu");

    while((socket_interrupt = crear_conexion(ip, puerto_interrupt)) <= 0){
        log_protegido("No se ha podido establecer la comunicacion con el Interrupt");
        sleep(5);
    }
    printf("Conexion con Interrupt Exitosa\n");
    

    while((socket_dispatch = crear_conexion(ip, puerto_dispatch)) <= 0){
        log_protegido("No se ha podido establecer la comunicacion con el Dispatch");
        sleep(5);
    }
    log_protegido("Conexion con Dispatcher Exitosa");

}
