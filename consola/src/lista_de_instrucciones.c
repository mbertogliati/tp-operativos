#include "../include/lista_de_instrucciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void procesar_linea(char **instruccion_leida, t_list *lista_de_instrucciones) {
	id identificador = get_identificador(instruccion_leida[0]);
	uint32_t *parametros = NULL;
	int i = 0;

	switch (identificador) {
	case NO_OP:
		for (; i < atoi(instruccion_leida[1]); i++)
			list_add(lista_de_instrucciones, crear_instruccion(identificador, 0, parametros));
		break;

	case IO: case READ: case WRITE: case COPY: case EXIT:
		while (instruccion_leida[i + 1]) {
			parametros = (uint32_t*) realloc(parametros, sizeof(uint32_t) * (i + 1));
			parametros[i] = atoi(instruccion_leida[i + 1]);
			i++;
		}
		list_add(lista_de_instrucciones, crear_instruccion(identificador, i, parametros));
		break;
	}
}

t_instruccion *crear_instruccion(id identificador, int cant_parametros, uint32_t *parametros) {
	t_instruccion *instruccion = (t_instruccion *) malloc(sizeof(t_instruccion));
	instruccion->identificador = identificador;
	instruccion->cant_parametros = cant_parametros;
	instruccion->parametros = parametros;
	return instruccion;
}

id get_identificador(char *identificador_leido) {
	id identificador = -1;
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

void leer_lista(t_list *lista_de_instrucciones) {
	if (!list_is_empty(lista_de_instrucciones)) {
		puts("\nLeyendo lista de instrucciones\n"
				"------------------------------\n");
		list_iterate(lista_de_instrucciones, (void *) leer_instruccion);
	}
}

void leer_instruccion(t_instruccion *instruccion) {
	id identificador = instruccion->identificador;
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

void liberar_lista(t_list *lista_de_instrucciones) {
	list_destroy_and_destroy_elements(lista_de_instrucciones, (void *) liberar_instruccion);
}

void liberar_instruccion(t_instruccion *instruccion) {
	free(instruccion->parametros);
	free(instruccion);
}
