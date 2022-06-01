#include "../include/conexionescpu.h"


void iniciar_conexion_memoria (){

    int socket_memoria;

    socket_memoria = crear_conexion(cpuconfig -> ip_memoria, cpuconfig -> puerto_memoria);  
    
    int* identificador = malloc(sizeof(int));
    *identificador = 0;
    send(socket_memoria, identificador, sizeof(int), 0);
    free (identificador);

    //[932][Tamaño][Entradas por tabla][Tamaño de pagina] son int
    if (recibir_operacion(socket_memoria) != 932){
        log_error(cpu_log, "ERROR - No se ha podido establecer una cominacion confiable con la Memoria");
        return; 
    }

    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer-> stream = recibir_buffer(&(buffer -> size), socket_memoria);

    entradas_por_tabla = sacar_de_buffer(buffer, sizeof(int)); // Hay que hacer un free cuando lo termine de usar
    tam_de_pagina = sacar_de_buffer(buffer, sizeof(int)); // Hay que hacer un free cuando lo termine de usar

};


void iniciar_conexion_dispatch(){
    //socket servidor
    int socket_dispatch; 

    socket_dispatch = iniciar_servidor(cpuconfig -> puerto_escucha_dispatch);
    //
    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer -> stream = recibir_buffer(&(buffer -> size), socket_dispatch);

};

void iniciar_conexion_interrupt(){
    //socket servidor 
    int socket_interrupt;
    socket_interrupt = iniciar_servidor( cpuconfig -> puerto_escucha_interrupt);

};
