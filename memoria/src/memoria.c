#include "../include/memoria.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    
    t_config_memoria* configuracion_memoria;
    configuracion_memoria = cargar_configuraciones("config/memoria.config");

    if(!configuracion_memoria)
        return EXIT_FAILURE;

    iniciar_conexion(configuracion_memoria);

    return EXIT_SUCCESS;
}
