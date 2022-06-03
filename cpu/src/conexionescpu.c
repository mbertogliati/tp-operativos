#include "../include/conexionescpu.h"

void *iniciar_conexion_memoria (void *arg){

    socket_memoria = crear_conexion(cpuconfig -> ip_memoria, cpuconfig -> puerto_memoria);  
    
    int* identificador = malloc(sizeof(int));
    *identificador = 0;
    send(socket_memoria, identificador, sizeof(int), 0);
    free (identificador);

    //[932][Tamaño][Entradas por tabla][Tamaño de pagina] son int
    if (recibir_operacion(socket_memoria) != 932){
        log_error(cpu_log, "ERROR - No se ha podido establecer una comunicacion confiable con la Memoria");
        return; 
    }

    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer-> stream = recibir_buffer(&(buffer -> size), socket_memoria);

    entradas_por_tabla = sacar_de_buffer(buffer, sizeof(int)); // Hay que hacer un free cuando lo termine de usar
    tam_de_pagina = sacar_de_buffer(buffer, sizeof(int)); // Hay que hacer un free cuando lo termine de usar
    return NULL;
};


void *iniciar_conexion_dispatch(void *arg){
    //socket servidor
    int servidor_dispatch; 
    t_buffer* buffer = malloc(sizeof(t_buffer));
    t_pcb *pcb;

    servidor_dispatch = iniciar_servidor(cpuconfig -> puerto_escucha_dispatch);
    //
    int socket_dispatch = esperar_cliente(servidor_dispatch);
    sem_wait(&mutex_interrupt);
    sem_post(&mutex_interrupt);
    while(true){
        buffer-> stream = recibir_buffer(&(buffer -> size), socket_dispatch);
        pcb = desempaquetar_pcb(buffer->stream);
        ciclo_de_instruccion(pcb);
    }

    free(buffer);
    return NULL;
};

void *iniciar_conexion_interrupt(void *arg){
    //socket servidor 
    int server_interrupt = iniciar_servidor( cpuconfig -> puerto_escucha_interrupt); ;
    socket_interrupt = esperar_cliente(server_interrupt);
    sem_init(&mutex_interrupt, 0, 1);
    bool *hay_interrupcion = malloc(sizeof(bool));
    while(true){
        recv(socket_interrupt, hay_interrupcion, siezof(hay_interrupcion), MSG_WAITALL);
        sem_wait(&mutex_interrupt);
        check_interrupt = *hay_interrupcion;
        sem_post(&mutex_interrupt);
    }

    free(hay_interrupcion);

};
void iniciar_conexiones(){

    iniciar_conexion_memoria(NULL);
    pthread_t dispatch;
    pthread_t interrupt;

    pthread_create(&dispatch, NULL, &iniciar_conexion_dispatch, NULL);
    pthread_create(&interrupt, NULL, &iniciar_conexion_interrupt, NULL);

    pthread_join(dispatch, NULL);
    pthread_join(interrupt, NULL);

}
