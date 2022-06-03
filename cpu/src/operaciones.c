#include "../include/operaciones.h"
bool pedir_escritura(int socket_memoria, int tam_escribir, void* dato_a_escribir, int direccion_fisica){
    
    bool* confirmacion = malloc(sizeof(bool));
    t_paquete *paq_instr_memoria = crear_paquete(ESCRIBIR);
    log_info(cpu_log, "Preparando paquete para memoria");
    agregar_a_paquete(paq_instr_memoria, &direccion_fisica, sizeof(int));
    agregar_a_paquete(paq_instr_memoria, &tam_escribir, sizeof(int));
    agregar_a_paquete(paq_instr_memoria, dato_a_escribir, tam_escribir);
    log_info(cpu_log, "Enviando paquete a memoria...");
    enviar_paquete(paq_instr_memoria, socket_memoria);
    eliminar_paquete(paq_instr_memoria);

    log_info(cpu_log, "Esperando respuesta...");
    recv(socket_memoria, confirmacion, sizeof(bool), MSG_WAITALL);
    log_info(cpu_log, "Respuesta recibida!!!");
    log_info(cpu_log, "La confirmacion recibida es: %d", *confirmacion);
    bool retorno = *confirmacion;
    free(confirmacion);
    return retorno;
}
void* pedir_lectura(int socket_memoria, int tam_leer, int direccion_fisica){
    void *valor_leido = malloc(tam_leer);

    t_paquete *paq_instr_memoria = crear_paquete(LEER);
    log_info(cpu_log, "Preparando paquete para memoria");
    agregar_a_paquete(paq_instr_memoria, &direccion_fisica, sizeof(int));
    agregar_a_paquete(paq_instr_memoria, &tam_leer, sizeof(int));
    log_info(cpu_log, "Enviando paquete a memoria...");
    enviar_paquete(paq_instr_memoria, socket_memoria);
    eliminar_paquete(paq_instr_memoria);

    log_info(cpu_log, "Esperando respuesta...");
    recv(socket_memoria, valor_leido, tam_leer, MSG_WAITALL);
    log_info(cpu_log, "Respuesta recibida!!!");
    return valor_leido;

}
int pedir_marco(int socket_memoria, int direccion_tabla_2, int entrada_lvl_2){
    int *marco = malloc(sizeof(int));

    t_paquete *paq_instr_memoria = crear_paquete(DEVOLVER_MARCO);
    log_info(cpu_log, "Preparando paquete para memoria");
    agregar_a_paquete(paq_instr_memoria, &direccion_tabla_2, sizeof(int));
    agregar_a_paquete(paq_instr_memoria, &entrada_lvl_2, sizeof(int));
    log_info(cpu_log, "Enviando paquete a memoria...");
    enviar_paquete(paq_instr_memoria, socket_memoria);
    eliminar_paquete(paq_instr_memoria);

    log_info(cpu_log, "Esperando respuesta...");
    recv(socket_memoria, marco, sizeof(int), MSG_WAITALL);
    log_info(cpu_log, "Respuesta recibida!!!");
    log_info(cpu_log, "El marco es: %d", *marco);
    int retorno = *marco;
    free(marco);
    return retorno;
}
int pedir_tabla_2(int socket_memoria, int tabla_del_proceso, int entrada_lvl_1){
    int *nro_tabla_2 = malloc(sizeof(int));
    t_paquete *paq_instr_memoria = crear_paquete(DEVOLVER_INDICE_TABLA_NVL2);
    log_info(cpu_log, "Preparando paquete para memoria");
    agregar_a_paquete(paq_instr_memoria, &tabla_del_proceso, sizeof(int));
    agregar_a_paquete(paq_instr_memoria, &entrada_lvl_1, sizeof(int));
    log_info(cpu_log, "Enviando paquete a memoria...");
    enviar_paquete(paq_instr_memoria, socket_memoria);
    eliminar_paquete(paq_instr_memoria);

    log_info(cpu_log, "Esperando respuesta...");
    recv(socket_memoria, nro_tabla_2, sizeof(int), MSG_WAITALL);
    log_info(cpu_log, "Respuesta recibida!!!");
    log_info(cpu_log, "El numero de tabla 2 es: %X", *nro_tabla_2);

    int retorno = *nro_tabla_2;
    free(nro_tabla_2);
    return retorno;
}
int obtener_direccion_fisica(int socket_memoria, int tabla_paginas, uint32_t direccion_logica, int entradas_por_tabla, int tam_pagina){

    int nro_de_pagina;
    int entrada_lvl_1;
    int entrada_lvl_2;
    int desplazamiento;
    int nro_tabla_2;
    int marco;

    nro_de_pagina = floor( ((double) direccion_logica) / ((double) tam_pagina));
    entrada_lvl_1 = floor( ((double) nro_de_pagina) / ((double) entradas_por_tabla));
    entrada_lvl_2 = nro_de_pagina % (entradas_por_tabla);
    desplazamiento = direccion_logica - nro_de_pagina * (tam_pagina);

    nro_tabla_2 = pedir_tabla_2(socket_memoria, tabla_paginas, entrada_lvl_1);
    marco = pedir_marco(socket_memoria, nro_tabla_2, entrada_lvl_2);
    return (marco*tam_pagina) + desplazamiento;
}