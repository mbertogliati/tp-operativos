#include "../include/conexiones.h"



int iniciar_conexiones(){

    int socket_servidor,
        socket_cliente,
        socket_cpu,
        socket_kernel;

    memoria_principal = malloc(configuracion->tam_memoria);
    socket_servidor = iniciar_servidor(configuracion->puerto);

    pthread_t comunicacion_cpu;
    pthread_t comunicacion_kernel;

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

    return NULL;
}
void *conectar_con_kernel(int *socket_kernel){

    printf("Aca se hace la conexion con KERNEL en el socket: %d\n", *socket_kernel);

    return NULL;
}
