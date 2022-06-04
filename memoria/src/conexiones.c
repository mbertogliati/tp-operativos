#include "../include/conexiones.h"

sem_t mutex_log;

void loguear_protegido (char* cadena, t_log_level level){
    switch(level){
        case LOG_LEVEL_ERROR:
            sem_wait(&mutex_log);
            log_error (memoria_log, "%s",cadena);
            sem_post(&mutex_log);
            break;
        case LOG_LEVEL_DEBUG:
            sem_wait(&mutex_log);
            log_debug (memoria_log,"%s", cadena);
            sem_post(&mutex_log);
            break;
        case LOG_LEVEL_INFO:
            sem_wait(&mutex_log);
            log_info (memoria_log, "%s",cadena);
            sem_post(&mutex_log);
            break;
        case LOG_LEVEL_TRACE:
            sem_wait(&mutex_log);
            log_trace (memoria_log,"%s", cadena);
            sem_post(&mutex_log);
            break;
        case LOG_LEVEL_WARNING:
            sem_wait(&mutex_log);
            log_warning (memoria_log,"%s", cadena);
            sem_post(&mutex_log);
            break;
    }
}

int iniciar_conexiones(){

    log_info(memoria_log, "Iniciando Conexiones...");
    int socket_servidor,
        socket_cliente,
        socket_cpu,
        socket_kernel;

    sem_init(&mutex_tabla_planificacion, 0, 1);

    log_info(memoria_log, "Iniciando servidor...");
    socket_servidor = iniciar_servidor(configuracion->puerto);
    if(socket_servidor == -1){
        log_error(memoria_log, "ERROR - No se pudo crear el servidor");
        return EXIT_FAILURE;
    }
    log_info(memoria_log, "Servidor Iniciado!!!");

    pthread_t comunicacion_cpu;
    pthread_t comunicacion_kernel;

    sem_init(&mutex_log, 0, 1);

    for(int i=0; i<2; i++){
        loguear_protegido("Esperando Cliente...", LOG_LEVEL_INFO);
        socket_cliente = esperar_cliente(socket_servidor);

        if (recibir_operacion(socket_cliente) == 0){
            loguear_protegido("Se conecto el CPU!!!", LOG_LEVEL_INFO);
            socket_cpu = socket_cliente;
            pthread_create(&comunicacion_cpu,NULL,&conectar_con_cpu,&socket_cpu);
        }
        
        else{
            loguear_protegido("Se conecto el KERNEL!!!", LOG_LEVEL_INFO);
            socket_kernel = socket_cliente;
            pthread_create(&comunicacion_kernel,NULL,&conectar_con_kernel,&socket_kernel);
        }
        sleep(1);
    }

    pthread_join(comunicacion_cpu, NULL);
    pthread_join(comunicacion_kernel, NULL);



    return EXIT_SUCCESS;
}

void *conectar_con_cpu(int* socket_cpu){

    cpu_log = log_create("memoria_cpu.log", "CPU", true, LOG_LEVEL_INFO);
    printf("Aca se hace la conexion con CPU en el socket: %d\n", *socket_cpu);
    log_info(cpu_log, "Enviando paquete de configuraciones al CPU...");
    t_paquete* paquete_configuraciones = crear_paquete(932);
    agregar_a_paquete(paquete_configuraciones, &(configuracion->entradas_por_tabla), sizeof(int));
    agregar_a_paquete(paquete_configuraciones, &(configuracion->tam_pagina), sizeof(int));
    enviar_paquete(paquete_configuraciones, *socket_cpu);
    eliminar_paquete(paquete_configuraciones);
    paquete_configuraciones=NULL;
    log_info(cpu_log, "Paquete enviado");

    int *instruccion_recibida = malloc(sizeof(int));
    t_buffer *buffer = malloc(sizeof(t_buffer));
    buffer -> size = 0;
    buffer -> stream = NULL;

    int *marco,
        *direccion_tabla2;

    int *indice,
        *direccion;
        
    int *direccion_fisica,
        *tam_leer,
        *tam_escribir;
    
    void *datos = NULL,
         *datos_leidos = NULL;

    bool *confirmacion;

    while(true){
        log_info(cpu_log, "Esperando peticion de CPU...");
        while(recv(*socket_cpu, instruccion_recibida, sizeof(int), MSG_WAITALL) <= 0){
                log_error(cpu_log, "ERROR - El cliente ha cerrado la conexion");
                return NULL;
        }
        log_info(cpu_log, "Instruccion de CPU leida!!!");

        buffer->stream = recibir_buffer(&(buffer->size), *socket_cpu);
        log_info(cpu_log, "Buffer Recibido!!!");
        switch(*instruccion_recibida){
            
            case DEVOLVER_INDICE_TABLA_NVL2:

                log_info(cpu_log, "El CPU quiere Tabla NVL 2");
                direccion = sacar_de_buffer(buffer, sizeof(int));
                indice = sacar_de_buffer(buffer, sizeof(int));
                direccion_tabla2 = malloc(sizeof(int));

                *direccion_tabla2 = obtener_tabla2(*direccion, *indice);
                send(*socket_cpu, direccion_tabla2, sizeof(int), 0);

                free(direccion_tabla2);
                free(indice);
                free(direccion);
                break;

            case DEVOLVER_MARCO:

                log_info(cpu_log, "El CPU quiere Nro de marco");
                direccion = sacar_de_buffer(buffer, sizeof(int));
                indice = sacar_de_buffer(buffer, sizeof(int));
                marco = malloc(sizeof(int));

                *marco = obtener_marco(*direccion, *indice);
                send(*socket_cpu, marco, sizeof(int), 0);

                free(marco);
                free(indice);
                free(direccion);
                break;              
            case LEER:
                
                log_info(cpu_log, "El CPU quiere LEER de memoria");
                direccion_fisica = sacar_de_buffer(buffer, sizeof(int));
                tam_leer = sacar_de_buffer(buffer, sizeof(int));
                datos_leidos = leer_de_memoria(*direccion_fisica, *tam_leer);
                send(*socket_cpu, datos_leidos, *tam_leer, 0);
                free(direccion_fisica);
                free(tam_leer);
                break;
                
            
            case ESCRIBIR:

                log_info(cpu_log, "El CPU quiere ESCRIBIR en memoria");
                direccion_fisica = sacar_de_buffer(buffer, sizeof(int));
                tam_escribir = sacar_de_buffer(buffer, sizeof(int));
                datos = sacar_de_buffer(buffer,*tam_escribir);

                escribir_a_memoria(*direccion_fisica, *tam_escribir, datos);
                confirmacion = malloc(sizeof(bool));
                *confirmacion = true;

                send(*socket_cpu, confirmacion, sizeof(bool), 0);

                free(direccion_fisica);
                free(tam_escribir);
                free(datos);
                free(confirmacion);

                break;

            default:
                log_error(cpu_log, "ERROR - La instruccion recibida del CPU es invalida");
                break;
            
        }
        free(buffer->stream); 
    }
    free(instruccion_recibida);
    free(buffer);
    return NULL;
}

void *conectar_con_kernel(int *socket_kernel){

    kernel_log = log_create("memoria_kernel.log", "KERNEL", true, LOG_LEVEL_INFO);
    printf("Aca se hace la conexion con KERNEL en el socket: %d\n", *socket_kernel);
    int *instruccion_recibida = malloc(sizeof(int));
    log_info(kernel_log, "Enviando mensaje de confirmacion...");
    bool *confirmacion = malloc(sizeof(bool));
    *confirmacion = true;
    t_buffer *buffer = malloc(sizeof(t_buffer));
    buffer->size = 0;
    buffer->stream = NULL;

    send(*socket_kernel, confirmacion, sizeof(bool), 0);
    free(confirmacion);
    log_info(kernel_log, "Mensaje enviado!!!");

    int *id_proceso,
        *tam_proceso;
        
    int *direccion_de_tabla = NULL;
    void *puntero_proceso = NULL;

    while(true){
        log_info(kernel_log, "Esperando Peticion de KERNEL...");
        while(recv(*socket_kernel, instruccion_recibida, sizeof(int), MSG_WAITALL) <= 0){
                log_error(kernel_log, "ERROR - El cliente ha cerrado la conexion");
                return NULL;
        }

        log_info(kernel_log, "Recibiendo buffer...");
        buffer->stream = recibir_buffer(&(buffer -> size), *socket_kernel);
        log_info(kernel_log, "Buffer Recibido!!!");

        
        
        confirmacion = NULL;

        switch(*instruccion_recibida){
            case CREAR_NUEVA_TABLA:

                log_info(kernel_log, "El kernel quiere agregar un proceso");
                log_info(kernel_log, "Recibiendo Proceso...");
                id_proceso = sacar_de_buffer(buffer, sizeof(int));
                tam_proceso = sacar_de_buffer(buffer, sizeof(int));
                direccion_de_tabla = malloc(sizeof(int));
                puntero_proceso = malloc(*tam_proceso);
                log_info(kernel_log, "Proceso recibido!!!");

                log_info(kernel_log, "Agregando proceso...");
                *direccion_de_tabla = agregar_proceso(*id_proceso, *tam_proceso, puntero_proceso);
                free(puntero_proceso);

                if(!(*direccion_de_tabla)){
                    log_error(kernel_log, "ERROR - No se ha podido agregar el proceso");    
                }
                log_info(kernel_log, "Proceso agregado exitosamente!!");
                log_info(kernel_log, "Respondiendo al kernel...");
                send(*socket_kernel, direccion_de_tabla, sizeof(int), 0);
                log_info(kernel_log, "Listo!");

                free(direccion_de_tabla);
                free(id_proceso);
                free(tam_proceso);
                break;

            case PROCESO_SUSPENDIDO:

                log_info(kernel_log, "El kernel quiere suspender un proceso");
                confirmacion = malloc(sizeof(bool));
                *confirmacion = true;

                log_info(kernel_log, "Recibiendo Proceso...");
                direccion_de_tabla = sacar_de_buffer(direccion_de_tabla, sizeof(int));
                log_info(kernel_log, "Proceso Recibido!!!");
                log_info(kernel_log, "Suspendiendo Proceso...");
                suspender_proceso2(*direccion_de_tabla);
                log_info(kernel_log, "Proceso Suspendido!!!");

                send(*socket_kernel, confirmacion, sizeof(bool), 0);

                free(direccion_de_tabla);
                break;

            case LIBERAR:

                log_info(kernel_log, "El kernel quiere eliminar un Proceso");
                confirmacion = malloc(sizeof(bool));
                *confirmacion = true;

                log_info(kernel_log, "Recibiendo proceso...");
                direccion_de_tabla = sacar_de_buffer(direccion_de_tabla, sizeof(int));
                log_info(kernel_log, "Proceso recibido!!!");
                finalizar_proceso(*direccion_de_tabla);

                send(*socket_kernel, confirmacion, sizeof(bool), 0);

                free(confirmacion);
                free(direccion_de_tabla);
                break;

            default:
                log_error(kernel_log, "ERROR - La instruccion recibida del KERNEL es invalida");
                break;
        }

    }

    free(instruccion_recibida);
    free(buffer);
    return NULL;
}
