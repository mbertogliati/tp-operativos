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

    printf("Aca se hace la conexion con CPU en el socket: %d\n", *socket_cpu);
    loguear_protegido("Enviando paquete de configuraciones al CPU...", LOG_LEVEL_INFO);
    t_paquete* paquete_configuraciones = crear_paquete(932);
    agregar_a_paquete(paquete_configuraciones, &(configuracion->entradas_por_tabla), sizeof(int));
    agregar_a_paquete(paquete_configuraciones, &(configuracion->tam_pagina), sizeof(int));
    enviar_paquete(paquete_configuraciones, *socket_cpu);
    eliminar_paquete(paquete_configuraciones);
    paquete_configuraciones=NULL;
    loguear_protegido("Paquete enviado!!!", LOG_LEVEL_INFO);

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
        loguear_protegido("Esperando peticion de CPU...", LOG_LEVEL_INFO);
        while(recv(*socket_cpu, instruccion_recibida, sizeof(int), MSG_WAITALL) <= 0){
                loguear_protegido("ERROR - El cliente ha cerrado la conexion", LOG_LEVEL_ERROR);
                return NULL;
        }
        loguear_protegido("Instruccion de CPU leida!!!", LOG_LEVEL_INFO);

        buffer->stream = recibir_buffer(&(buffer->size), *socket_cpu);
        loguear_protegido("Buffer Recibido!!!", LOG_LEVEL_INFO);
        switch(*instruccion_recibida){
            
            case DEVOLVER_INDICE_TABLA_NVL2:

                loguear_protegido("El CPU quiere Tabla NVL 2", LOG_LEVEL_INFO);
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

                loguear_protegido("El CPU quiere Nro de Marco", LOG_LEVEL_INFO);
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
                
                loguear_protegido("El CPU quiere LEER de memoria", LOG_LEVEL_INFO);
                direccion_fisica = sacar_de_buffer(buffer, sizeof(int));
                tam_leer = sacar_de_buffer(buffer, sizeof(int));
                datos_leidos = leer_de_memoria(*direccion_fisica, *tam_leer);
                send(*socket_cpu, datos_leidos, *tam_leer, 0);
                free(direccion_fisica);
                free(tam_leer);
                break;
                
            
            case ESCRIBIR:

                loguear_protegido("El CPU quiere ESCRIBIR en memoria", LOG_LEVEL_INFO);
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
                loguear_protegido("ERROR - La instruccion recibida del CPU es invalida", LOG_LEVEL_ERROR);
                break;
            
        }
        free(buffer->stream); 
    }
    free(instruccion_recibida);
    free(buffer);
    return NULL;
}
void *conectar_con_kernel(int *socket_kernel){

    printf("Aca se hace la conexion con KERNEL en el socket: %d\n", *socket_kernel);
    int *instruccion_recibida = malloc(sizeof(int));
    loguear_protegido("Enviando mensaje de confirmacion...", LOG_LEVEL_INFO);
    bool *confirmacion = malloc(sizeof(bool));
    *confirmacion = true;
    t_buffer *buffer = malloc(sizeof(t_buffer));
    buffer->size = 0;
    buffer->stream = NULL;

    send(*socket_kernel, confirmacion, sizeof(bool), 0);
    free(confirmacion);
    loguear_protegido("Mensaje enviado!!!", LOG_LEVEL_INFO);

    int *id_proceso,
        *tam_proceso;
        
    int *direccion_de_tabla = NULL;
    void *puntero_proceso = NULL;

    while(true){
        loguear_protegido("Esperando Peticion de KERNEL...", LOG_LEVEL_INFO);
        while(recv(*socket_kernel, instruccion_recibida, sizeof(int), MSG_WAITALL) <= 0){
                loguear_protegido("ERROR - El cliente ha cerrado la conexion", LOG_LEVEL_ERROR);
                return NULL;
        }

        loguear_protegido("Recibiendo buffer...", LOG_LEVEL_INFO);
        buffer->stream = recibir_buffer(&(buffer -> size), *socket_kernel);
        loguear_protegido("Buffer Recibido!!!", LOG_LEVEL_INFO);

        
        
        confirmacion = NULL;

        switch(*instruccion_recibida){
            case CREAR_NUEVA_TABLA:

                id_proceso = sacar_de_buffer(buffer, sizeof(int));
                tam_proceso = sacar_de_buffer(buffer, sizeof(int));
                direccion_de_tabla = malloc(sizeof(int));
                puntero_proceso = malloc(*tam_proceso);

                *direccion_de_tabla = agregar_proceso(*id_proceso, *tam_proceso, puntero_proceso);
                free(puntero_proceso);

                if(!(*direccion_de_tabla)){
                    loguear_protegido("ERROR - No se ha podido agregar el proceso", LOG_LEVEL_ERROR);    
                }
                loguear_protegido("Proceso creado exitosamente!!", LOG_LEVEL_INFO);
                loguear_protegido("Respondiendo al kernel...", LOG_LEVEL_INFO);
                send(*socket_kernel, direccion_de_tabla, sizeof(int), 0);
                loguear_protegido("Listo!", LOG_LEVEL_INFO);

                free(direccion_de_tabla);
                free(id_proceso);
                free(tam_proceso);
                break;

            case PROCESO_SUSPENDIDO:

                confirmacion = malloc(sizeof(bool));
                *confirmacion = true;

                direccion_de_tabla = sacar_de_buffer(direccion_de_tabla, sizeof(int));

                suspender_proceso2(*direccion_de_tabla);

                send(*socket_kernel, confirmacion, sizeof(bool), 0);

                free(direccion_de_tabla);
                break;

            case LIBERAR:

                confirmacion = malloc(sizeof(bool));
                *confirmacion = true;

                direccion_de_tabla = sacar_de_buffer(direccion_de_tabla, sizeof(int));

                finalizar_proceso(*direccion_de_tabla);

                send(*socket_kernel, confirmacion, sizeof(bool), 0);

                free(confirmacion);
                free(direccion_de_tabla);
                break;

            default:
                loguear_protegido("ERROR - La instruccion recibida del KERNEL es invalida", LOG_LEVEL_ERROR);
                break;
        }

    }

    free(instruccion_recibida);
    free(buffer);
    return NULL;
}
