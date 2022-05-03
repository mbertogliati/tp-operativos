#include "../include/memoria.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    int tam_memoria,
        tam_pagina,
        entradas_por_tabla;
    void* memoria_principal;
    
    configuracion_memoria = cargar_configuraciones("config/memoria.config");

    if(!configuracion_memoria)
        return EXIT_FAILURE;

    //iniciar_conexion(configuracion_memoria);

    funcion_test1();

    //t_tabla tablas_de_paginas[entradas_por_tabla][entradas_por_tabla];
    //No se puede usar parametros para crear arrays o tablas de tamaño variable
    //memset(tablas_de_paginas,0,entradas_por_tabla*entradas_por_tabla*sizeof(t_tabla));

    //memoria_principal = malloc(tam_memoria);


    return EXIT_SUCCESS;
}

void funcion_test1(){
	agregar_proceso(0, 16);
	agregar_proceso(1, 16);
	agregar_proceso(2, 16);

	t_tabla2* tabla_test = encontrar_tabla2(0, 1, 0);
	tabla_test->M = 1;
	tabla_test->P = 1;
	tabla_test = encontrar_tabla2(1, 0, 1);
	tabla_test->marco = 55;

	tabla_test = encontrar_tabla2(0, 1, 0);
	if(tabla_test->M)
		printf("M correcto");
	if(tabla_test->P)
		printf("P correcto");
	if(tabla_test->U)
		printf("U correcto");

	tabla_test = encontrar_tabla2(1, 0, 1);
	printf("%d", tabla_test->marco);
}
