#include "../include/conexiones.h"

sem_t mutex_log;

int iniciar_conexiones(){

    int socket_servidor,
        socket_cliente,
        socket_cpu,
        socket_kernel;

    memoria_principal = malloc(configuracion->tam_memoria);
    socket_servidor = iniciar_servidor(configuracion->puerto);

    pthread_t comunicacion_cpu;
    pthread_t comunicacion_kernel;

    sem_init(&mutex_log, 0, 1);

    for(int i=0; i<2; i++){

        socket_cliente = esperar_cliente(socket_servidor);

        if (recibir_operacion(socket_cliente) == 0){
            socket_cpu = socket_cliente;
            pthread_create(&comunicacion_cpu,NULL,&conectar_con_cpu,&socket_cpu);
        }
        
        else{
            socket_kernel = socket_cliente;
            pthread_create(&comunicacion_kernel,NULL,&conectar_con_kernel,&socket_kernel);
        }
    }

    pthread_join(comunicacion_cpu, NULL);
    pthread_join(comunicacion_kernel, NULL);



    return EXIT_SUCCESS;
}
void *conectar_con_cpu(int* socket_cpu){

    printf("Aca se hace la conexion con CPU en el socket: %d\n", *socket_cpu);
    t_paquete* paquete_configuraciones = crear_paquete(-1);
    agregar_a_paquete(paquete_configuraciones, &(configuracion->entradas_por_tabla), sizeof(int));
    agregar_a_paquete(paquete_configuraciones, &(configuracion->tam_pagina), sizeof(int));
    enviar_paquete(paquete_configuraciones, *socket_cpu);
    eliminar_paquete(paquete_configuraciones);
    paquete_configuraciones=NULL;

    inst_memoria instruccion_recibida;
    t_buffer *buffer = malloc(sizeof(t_buffer));

    while(true){
        instruccion_recibida = recibir_operacion(*socket_cpu);
        if(instruccion_recibida < 0){
            
            break;
        }

        buffer->stream = recibir_buffer(&(buffer->size), socket_cpu);

        switch(instruccion_recibida){
            case DEVOLVER_INDICE_TABLA_NVL2:
            case DEVOLVER_MARCO:
            case LEER:
            case ESCRIBIR:
            case LIBERAR:
            default:
            
        }
        free(buffer);
    }

    return NULL;
}
void *conectar_con_kernel(int *socket_kernel){

    printf("Aca se hace la conexion con KERNEL en el socket: %d\n", *socket_kernel);

    return NULL;
}
void loguear_protegido (char* cadena, t_log_level level){
    switch(level){
        case LOG_LEVEL_ERROR:
            sem_wait(&mutex_log);
            log_error (memoria_log, cadena);
            sem_post(&mutex_log);   
        case LOG_LEVEL_DEBUG:
            sem_wait(&mutex_log);
            log_debug (memoria_log, cadena);
            sem_post(&mutex_log);  
        case LOG_LEVEL_INFO:
            sem_wait(&mutex_log);
            log_info (memoria_log, cadena);
            sem_post(&mutex_log);  
        case LOG_LEVEL_TRACE:
            sem_wait(&mutex_log);
            log_trace (memoria_log, cadena);
            sem_post(&mutex_log);  
        case LOG_LEVEL_WARNING:
            sem_wait(&mutex_log);
            log_warning (memoria_log, cadena);
            sem_post(&mutex_log);
    }
}
