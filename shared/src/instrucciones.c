#include "../include/instrucciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_instruccion *crear_instruccion(int identificador, int cant_parametros, uint32_t *parametros) {
	t_instruccion *instruccion = (t_instruccion *) malloc(sizeof(t_instruccion));
	instruccion->identificador = identificador;
	instruccion->cant_parametros = cant_parametros;
	instruccion->parametros = parametros;
	return instruccion;
}

int get_identificador(char *identificador_leido) {
	int identificador = -1;
	if (!strcmp("NO_OP", identificador_leido))
		identificador = NO_OP;
	if (!strcmp("I/O", identificador_leido))
		identificador = IO;
	if (!strcmp("READ", identificador_leido))
		identificador = READ;
	if (!strcmp("COPY", identificador_leido))
		identificador = COPY;
	if (!strcmp("WRITE", identificador_leido))
		identificador = WRITE;
	if (!strcmp("EXIT", identificador_leido))
		identificador = EXIT;
	return identificador;
}

void leer_instruccion(t_instruccion *instruccion) {
	int identificador = instruccion->identificador;
	int cant_parametros = instruccion->cant_parametros;
	uint32_t *parametros = instruccion->parametros;

	printf("identificador: %d\nparametros: ", identificador);

	if (!parametros)
		puts("no hay parametros\n");

	for (int i = 0; i < cant_parametros; i++) {
		if (i == cant_parametros - 1)
			printf("%d\n\n", parametros[i]);
		else
			printf("%d, ", parametros[i]);
	}
}

void leer_lista(t_list *lista_de_instrucciones) {
	if (!list_is_empty(lista_de_instrucciones)) {
		puts("\nLeyendo lista de instrucciones\n"
				"------------------------------\n");
		list_iterate(lista_de_instrucciones, (void *) leer_instruccion);
	}
}

void liberar_instruccion(t_instruccion *instruccion) {
	free(instruccion->parametros);
	free(instruccion);
}

void liberar_lista(t_list *lista_de_instrucciones) {
	list_destroy_and_destroy_elements(lista_de_instrucciones, (void *) liberar_instruccion);
}
