#include "../include/memoria.h"


void imprimir_puntero_void(void* a_imprimir, int tamanio_a_imprimir){
	for(int i = 0; i < tamanio_a_imprimir; i++)
		printf("%x", ((unsigned char*) a_imprimir)[i]);
	printf("\n");
}

void funcion_test1(){
	void* proceso = malloc(16);
	imprimir_puntero_void(proceso, 16);
	imprimir_puntero_void(proceso, configuracion->tam_pagina);
	int proceso1 = agregar_proceso(1, 16, proceso);
	int proceso2 = agregar_proceso(2, 33, proceso);
	free(proceso);
	proceso = malloc(12);
	int proceso3 = agregar_proceso(3, 12, proceso);
	free(proceso);

	int tabla2 = obtener_tabla2(proceso1, 1);
	printf("%d\n", tabla2);
	int marco = obtener_marco(tabla2, 1);
	printf("%d\n", marco);
	int direccion = (marco * configuracion->tam_pagina);

	proceso = leer_de_memoria(direccion, configuracion->tam_pagina);
	imprimir_puntero_void(proceso, configuracion->tam_pagina);
	free(proceso);

//	printf("Direccion tabla1 proceso 1: %d\n", proceso1);
//	printf("Direccion tabla1 proceso 2: %d\n", proceso2);
//	printf("Direccion tabla1 proceso 3: %d\n", proceso3);
//	printf("direccion tabla2: %d\n", tabla2);
}

int main() {

    configuracion = NULL;
	
    iniciar_estructuras();
	//iniciar_conexiones(configuracion);

    if(!configuracion)
        return EXIT_FAILURE;

    printf("El puerto es: %s\n", configuracion->puerto);
    
    //iniciar_conexion(configuracion_memoria);

    funcion_test1();

    //t_tabla tablas_de_paginas[entradas_por_tabla][entradas_por_tabla];
    //No se puede usar parametros para crear arrays o tablas de tamaño variable
    //memset(tablas_de_paginas,0,entradas_por_tabla*entradas_por_tabla*sizeof(t_tabla));

    //memoria_principal = malloc(tam_memoria);

    return EXIT_SUCCESS;
}
