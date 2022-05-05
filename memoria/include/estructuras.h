#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <stdlib.h>
#include <math.h>

t_log *memoria_log;
void* memoria_principal;
t_list* tablas;

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

t_config_memoria* configuracion;

typedef struct{
    int marco;
    char P;
    char U;
    char M;
} t_tabla2;

void cargar_configuraciones(char* path);
bool es_configuracion_valida(t_config *config_memoria);
int agregar_proceso(int tamanio_proceso);
int obtener_tabla2(int direccion, int indice);

#endif
