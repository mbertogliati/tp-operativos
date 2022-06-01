#include "../include/conexionescpu.h"


void iniciar_conexion_memoria (){

    int socket_memoria;
    void* buffer;

    socket_memoria = crear_conexion(cpuconfig -> ip_memoria, cpuconfig -> puerto_memoria);  
    
    int* identificador = malloc(sizeof(int));
    *identificador = 0;
    send(socket_memoria, identificador, sizeof(int), 0);
    free (identificador);

    //[932][Tamaño][Entradas por tabla][Tamaño de pagina] son int
    if (recibir_operacion(socket_memoria) != 932){
        //error
        return; 
    }

    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer -> stream = recibir_buffer(&(buffer -> size), socket_memoria);

    int* entradas_por_tabla = sacar_de_buffer(buffer, sizeof(int)); // Hay que hacer un free cuando lo termine de usar
    int* tam_de_pagina = sacar_de_buffer(buffer, sizeof(int)); // Hay que hacer un free cuando lo termine de usar

};


void iniciar_conexion_dispatch(){
    //socket servidor
    socket_dispatch; 
    void* buffer;

    socket_dispatch = iniciar_servidor(cpuconfig -> puerto_escucha_dispatch);
    
    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer -> stream = recibir_buffer(&(buffer -> size), socket_dispatch);

};

void iniciar_conexion_interrupt(){
    //socket servidor 
    int socket_interrupt;
    socket_interrupt = iniciar_servidor( cpuconfig -> puerto_ecucha_interrupt);

};
