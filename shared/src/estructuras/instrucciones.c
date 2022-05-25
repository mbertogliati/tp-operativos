#include "../../include/estructuras/instrucciones.h"
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

int get_cant_parametros(int identificador) {
	int cant_parametros = 0;
	switch (identificador) {
	case EXIT:
		break;

	case NO_OP: case IO: case READ:
		cant_parametros = 1;
		break;

	case WRITE: case COPY:
		cant_parametros = 2;
		break;
	}
	return cant_parametros;
}

void imprimir_instruccion(t_instruccion *instruccion) {
	int identificador = instruccion->identificador;
	int cant_parametros = instruccion->cant_parametros;
	uint32_t *parametros = instruccion->parametros;

	printf("\tidentificador: %d\n\tparametros: ", identificador);

	if (!parametros)
		puts("no hay parametros\n");

	for (int i = 0; i < cant_parametros; i++) {
		if (i == cant_parametros - 1)
			printf("%d\n\n", parametros[i]);
		else
			printf("%d, ", parametros[i]);
	}
}

void liberar_instruccion(t_instruccion *instruccion) {
	free(instruccion->parametros);
	free(instruccion);
}

t_instruccion *desempaquetar_instruccion(void *buffer, int *desplazamiento) {
	int identificador, cant_parametros, tam_parametros;
	uint32_t *parametros;

	memcpy(&identificador, buffer + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);

	memcpy(&cant_parametros, buffer + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);

	if ((tam_parametros = cant_parametros * sizeof(uint32_t)))
		parametros = (uint32_t *) malloc(tam_parametros);
	else
		parametros = NULL;

	memcpy(parametros, buffer + *desplazamiento, tam_parametros);
	*desplazamiento += tam_parametros;

	return crear_instruccion(identificador, cant_parametros, parametros);
}
