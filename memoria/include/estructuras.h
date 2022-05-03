#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <stdlib.h>
#include "paginacion.h"
#include <math.h>

t_log *memoria_log;

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

t_config_memoria* configuracion_memoria;
t_list* procesos;

t_config_memoria *cargar_configuraciones(char** path);
bool es_configuracion_valida(t_config *config_memoria);
void agregar_proceso(int id_proceso, int tamanio_proceso);
t_tabla2* encontrar_tabla2(int proceso_id, int entrada_primer_nivel, int entrada_segundo_nivel);


#endif
