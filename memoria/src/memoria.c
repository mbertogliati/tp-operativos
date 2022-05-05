#include "../include/memoria.h"


void funcion_test1(){
	tablas = list_create();
	int proceso1 = agregar_proceso(16);
	int proceso2 = agregar_proceso(17);
	int proceso3 = agregar_proceso(16);

	int tabla2 = obtener_tabla2(proceso1, 1);

	printf("Direccion tabla1 proceso 1: %d\n", proceso1);
	printf("Direccion tabla1 proceso 2: %d\n", proceso2);
	printf("Direccion tabla1 proceso 3: %d\n", proceso3);
	printf("direccion tabla2: %d\n", tabla2);
}

int main() {
    
    cargar_configuraciones("config/memoria.config");

    if(!configuracion)
        return EXIT_FAILURE;

    //iniciar_conexion(configuracion_memoria);

    funcion_test1();

    //t_tabla tablas_de_paginas[entradas_por_tabla][entradas_por_tabla];
    //No se puede usar parametros para crear arrays o tablas de tamaño variable
    //memset(tablas_de_paginas,0,entradas_por_tabla*entradas_por_tabla*sizeof(t_tabla));

    //memoria_principal = malloc(tam_memoria);

    return EXIT_SUCCESS;
}
