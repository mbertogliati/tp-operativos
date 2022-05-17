#include "../memoria/include/conexiones.h"
int main(void){
    t_log *test_cpu;
    test_cpu = log_create ("test_cpu.log", "CPU_TEST", true, LOG_LEVEL_INFO);
    log_info(test_cpu, "Iniciando test de CPU...");

    char ip[]="127.0.0.1",
        puerto_memoria[]="8002";

    int socket_memoria;
    socket_memoria = crear_conexion(ip,puerto_memoria);

    while(socket_memoria == -1){
        log_warning(test_cpu, "No se ha podido conectar a memoria. Reintentando...");
        sleep(5);
        socket_memoria = crear_conexion(ip,puerto_memoria);
    }
    log_info(test_cpu, "Conexion exitosa!!!");

    int handshake = 0,
        operacion,
        *entradas_por_tabla,
        *tam_pagina;
    
    t_buffer *buffer = malloc(sizeof(t_buffer));

    log_info(test_cpu, "Enviando mensaje...");
    send(socket_memoria, &handshake, sizeof(int), 0);
    log_info(test_cpu, "Mensaje enviado!");
    log_info(test_cpu, "Esperando respuesta...");
    operacion = recibir_operacion(socket_memoria);
    log_info(test_cpu, "Respuesta recibida!!!");

    printf("El valor de operacion es %d\n", operacion);
    if (operacion == 932)
        log_info(test_cpu, "Conexion exitosa!!!");
    else{
        log_error(test_cpu, "Ha habido un error en la comunicacion");
        return EXIT_FAILURE;
    }

    log_info(test_cpu, "Recibiendo buffer...");
    buffer -> stream = recibir_buffer(&(buffer->size), socket_memoria);
    log_info(test_cpu, "Buffer Recibido!");

    printf("Size del buffer es %d \n", buffer -> size);
    log_info(test_cpu, "Sacando datos del buffer...");

    entradas_por_tabla = sacar_de_buffer(buffer, sizeof(int));
    tam_pagina = sacar_de_buffer(buffer, sizeof(int));

    log_info(test_cpu, "Datos extraidos!!!");
    printf("Las paginas son de %d bytes y hay %d entradas por tabla", *tam_pagina, *entradas_por_tabla);



    free(entradas_por_tabla);
    free(tam_pagina);
    free(buffer);
    liberar_conexion(socket_memoria);
    return EXIT_SUCCESS;
}
void *simulacion_kernel(void* nada){
    return NULL;
}
