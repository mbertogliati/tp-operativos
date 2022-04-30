#include "../include/memoria.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    int tam_memoria,
        tam_pagina,
        entradas_por_tabla;
    void* memoria_principal;
    
    t_config_memoria* configuracion_memoria;
    configuracion_memoria = cargar_configuraciones("config/memoria.config");

    if(!configuracion_memoria)
        return EXIT_FAILURE;

    iniciar_conexion(configuracion_memoria);

    t_tabla tablas_de_paginas[entradas_por_tabla][entradas_por_tabla];
    memset(tablas_de_paginas,0,entradas_por_tabla*entradas_por_tabla*sizeof(t_tabla));

    memoria_principal = malloc(tam_memoria);


    return EXIT_SUCCESS;
}
