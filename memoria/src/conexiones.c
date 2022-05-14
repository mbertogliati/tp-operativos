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
            loguear_protegido("ERROR - No se ha podido leer la instruccion del CPU", LOG_LEVEL_ERROR);
            break;
        }

        buffer->stream = recibir_buffer(&(buffer->size), socket_cpu);

        switch(instruccion_recibida){
            case DEVOLVER_INDICE_TABLA_NVL2:
                {
                    int *indice,
                        *direccion;
                    direccion = sacar_de_buffer(buffer, sizeof(int));
                    indice = sacar_de_buffer(buffer, sizeof(int));

                    int direccion_tabla2 = obtener_tabla2(*direccion, *indice);
                    send(socket_cpu, &direccion_tabla2, sizeof(int), 0);
                    free(indice);
                    free(direccion);
                    break;
                }

            case DEVOLVER_MARCO:
                {
                    int *indice,
                        *direccion;
                    direccion = sacar_de_buffer(buffer, sizeof(int));
                    indice = sacar_de_buffer(buffer, sizeof(int));

                    int marco = obtener_marco(*direccion, *indice);
                    send(socket_cpu, &marco, sizeof(int), 0);
                    free(indice);
                    free(direccion);
                    break; 
                }               
            case LEER:
                {
                    int *direccion_fisica,
                        *tam_leer;

                    direccion_fisica = sacar_de_buffer(buffer, sizeof(int));
                    tam_leer = sacar_de_buffer(buffer, sizeof(int));
                    void* datos_leidos = leer_de_memoria(*direccion_fisica, *tam_leer);
                    send(socket_cpu, datos_leidos, *tam_leer, 0);
                    free(direccion_fisica);
                    free(tam_leer);
                    break;
                }
            
            case ESCRIBIR:
                {
                    int *direccion_fisica,
                        *tam_escribir;
                    void* datos = NULL;

                    direccion_fisica = sacar_de_buffer(buffer, sizeof(int));
                    tam_escribir = sacar_de_buffer(buffer, sizeof(int));
                    datos = sacar_de_buffer(buffer,*tam_escribir);

                    escribir_a_memoria(*direccion_fisica, *tam_escribir, datos);
                    bool *confirmacion = malloc(sizeof(bool));
                    *confirmacion = true;

                    send(socket_cpu, confirmacion, sizeof(bool), 0);

                    free(direccion_fisica);
                    free(tam_escribir);
                    free(datos);
                    free(confirmacion);

                    break;
                }

            default:
                loguear_protegido("ERROR - La instruccion recibida del CPU es invalida", LOG_LEVEL_ERROR);
                break;
            
        }
        free(buffer->stream); 
    }
    free(buffer);
    return NULL;
}
void *conectar_con_kernel(int *socket_kernel){

    printf("Aca se hace la conexion con KERNEL en el socket: %d\n", *socket_kernel);
    inst_memoria instruccion_recibida;
    t_buffer *buffer = malloc(sizeof(t_buffer));

    while(true){

        instruccion_recibida = recibir_operacion(*socket_kernel);
        if(instruccion_recibida < 0){
            loguear_protegido("ERROR - No se ha podido leer la instruccion del KERNEL", LOG_LEVEL_ERROR);
            break;
        }

        buffer->stream = recibir_buffer(&(buffer->size), socket_kernel);

        switch(instruccion_recibida){
            case CREAR_NUEVA_TABLA:
                {
                    int *id_proceso,
                        *tam_proceso,
                        direccion_tabla;

                    id_proceso = sacar_de_buffer(buffer, sizeof(int));
                    tam_proceso = sacar_de_buffer(buffer, sizeof(int));

                    direccion_tabla = agregar_proceso(*id_proceso, *tam_proceso, NULL);
                    if(!direccion_tabla){
                        loguear_protegido("ERROR - No se ha podido agregar el proceso", LOG_LEVEL_ERROR);    
                    }
                    
                    send(socket_kernel, &direccion_tabla, sizeof(int), 0);

                    free(id_proceso);
                    free(tam_proceso);
                    break;
                }

            case PROCESO_SUSPENDIDO:
                {
                    int* direccion_de_tabla;
                    bool confirmacion = true;
                    direccion_de_tabla = sacar_de_buffer(direccion_de_tabla, sizeof(int));

                    suspender_proceso2(*direccion_de_tabla);

                    send(socket_kernel, &confirmacion, sizeof(bool), 0);

                    free(direccion_de_tabla);
                    break;
                }
            case LIBERAR:
                {
                    int* direccion_de_tabla;
                    bool confirmacion = true;
                    direccion_de_tabla = sacar_de_buffer(direccion_de_tabla, sizeof(int));

                    finalizar_proceso(*direccion_de_tabla);

                    send(socket_kernel, &confirmacion, sizeof(bool), 0);

                    free(direccion_de_tabla);
                    break;
                }
            default:
                loguear_protegido("ERROR - La instruccion recibida del KERNEL es invalida", LOG_LEVEL_ERROR);
                break;
        }

    }

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
