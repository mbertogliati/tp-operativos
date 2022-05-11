#include "../include/estructuras.h"

t_config_memoria *cargar_configuraciones(char* path){
    t_config *config_memoria = NULL;
    t_config_memoria *configuracion = malloc(sizeof(t_config_memoria));

    memoria_log = log_create("logs/memoria.log","MEMORIA",true,LOG_LEVEL_INFO);
    log_info(memoria_log, "Leyendo archivo de Configuraciones\n");

    if(!(config_memoria = config_create(path))){
        log_error(memoria_log,"ERROR - No se encontro el archivo de configuracion de memoria\n");
        return NULL;
    }

    if(!es_configuracion_valida(config_memoria))
        return NULL;
    

    configuracion->tam_memoria = config_get_int_value(config_memoria,"TAM_MEMORIA");
    configuracion->tam_pagina = config_get_int_value(config_memoria,"TAM_PAGINA");
    configuracion->entradas_por_tabla = config_get_int_value(config_memoria,"ENTRADAS_POR_TABLA");
    configuracion->retardo_memoria = config_get_int_value(config_memoria,"RETARDO_MEMORIA");
    configuracion->retardo_swap = config_get_int_value(config_memoria,"RETARDO_SWAP");
    configuracion->marcos_por_proceso = config_get_int_value(config_memoria,"MARCOS_POR_PROCESO");

    configuracion->puerto = config_get_string_value(config_memoria,"PUERTO_ESCUCHA");

    configuracion->algoritmo_reemplazo = config_get_string_value(config_memoria,"ALGORITMO_REEMPLAZO");

    configuracion->path_swap = config_get_string_value(config_memoria,"PATH_SWAP");
    
    log_info(memoria_log, "Listo!\n");

    return configuracion;
}
bool es_configuracion_valida(t_config *config_memoria){
    char claves_validas[9][20+1] = {"TAM_MEMORIA","TAM_PAGINA","ENTRADAS_POR_TABLA",
                                "RETARDO_MEMORIA","RETARDO_SWAP","MARCOS_POR_PROCESO",
                                "PUERTO_ESCUCHA","ALGORITMO_REEMPLAZO","PATH_SWAP"};
    for(int i = 0; i < 0; i++){
        if (!config_has_property(config_memoria, claves_validas[i])){
            log_error(memoria_log,"ERROR - Archivo de configuracion invalido\n");
            return false;
        }
    }
    if (config_keys_amount(config_memoria) >9 ){
        log_error(memoria_log,"ERROR - Archivo de configuracion invalido\n");
        return false;
    }
    return true;
}
