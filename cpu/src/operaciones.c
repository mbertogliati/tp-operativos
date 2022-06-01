#include "../include/operaciones.h"
bool pedir_escritura(int socket_memoria, int tam_escribir, uint32_t dato_a_escribir, int direccion_fisica){
    
    bool* confirmacion = malloc(sizeof(bool));
    t_paquete *paq_instr_memoria = crear_paquete(ESCRIBIR);
    log_info(cpu_log, "Preparando paquete para memoria");
    agregar_a_paquete(paq_instr_memoria, &direccion_fisica, sizeof(int));
    agregar_a_paquete(paq_instr_memoria, &tam_escribir, sizeof(int));
    agregar_a_paquete(paq_instr_memoria, &dato_a_escribir, sizeof(uint32_t));
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
uint32_t pedir_lectura(int socket_memoria, int tam_leer, int direccion_fisica){
    uint32_t *valor_leido = malloc(sizeof(int));

    t_paquete *paq_instr_memoria = crear_paquete(LEER);
    log_info(cpu_log, "Preparando paquete para memoria");
    agregar_a_paquete(paq_instr_memoria, &direccion_fisica, sizeof(int));
    agregar_a_paquete(paq_instr_memoria, &tam_leer, sizeof(int));
    log_info(cpu_log, "Enviando paquete a memoria...");
    enviar_paquete(paq_instr_memoria, socket_memoria);
    eliminar_paquete(paq_instr_memoria);

    log_info(cpu_log, "Esperando respuesta...");
    recv(socket_memoria, valor_leido, sizeof(uint32_t), MSG_WAITALL);
    log_info(cpu_log, "Respuesta recibida!!!");
    log_info(cpu_log, "El valor leido es: %d", *valor_leido);
    int retorno = *valor_leido;
    free(valor_leido);
    return retorno;

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