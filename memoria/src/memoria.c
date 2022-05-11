#include "../include/memoria.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    
    configuracion_memoria = NULL;
    configuracion_memoria = cargar_configuraciones("config/memoria.config");

    if(!configuracion_memoria)
        return EXIT_FAILURE;

    printf("El puerto es: %s\n", configuracion_memoria->puerto);

    iniciar_conexiones(configuracion_memoria);

    return EXIT_SUCCESS;
}
