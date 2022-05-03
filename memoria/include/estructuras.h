#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <stdlib.h>

t_log *memoria_log;
void* memoria_principal;
t_list *tablas;

typedef struct{
    char* puerto;
    int tam_memoria;
    int tam_pagina;
    int entradas_por_tabla;
    int retardo_memoria;
    int marcos_por_proceso;
    char* algoritmo_reemplazo;
    int retardo_swap;
    char* path_swap;
} t_config_memoria;

typedef struct{
    int marco;
    char P;
    char U;
    char M;
} t_tabla;

t_config_memoria *cargar_configuraciones(char** path);
bool es_configuracion_valida(t_config *config_memoria);


#endif