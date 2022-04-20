#include "../include/consola.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //getcwd()
#include <commons/string.h>

int main() {
	char cwd[TAM];
	char *path = string_new();

	if (getcwd(cwd, TAM)) {
		string_append(&path, cwd);
		string_append(&path, "/pseudocodigo.txt");
	}
	
	consola(path, 0);
}

void consola(char *path, int tamanio) {
	t_list *lista_de_instrucciones = list_create();
	leer_archivo(path, lista_de_instrucciones);
	leer_lista(lista_de_instrucciones);
	liberar_lista(lista_de_instrucciones);
}

void leer_archivo(char *path, t_list *lista_de_instrucciones) {
	FILE * fp = fopen(path, "r");

	if (!fp) {
		puts("no se encontro el archivo\n");
		return;
	}

	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	t_instruccion *instruccion = NULL;

	while ((read = getline(&line, &len, fp)) != -1) {
		instruccion = crear_instruccion(string_split(line, " "));
		list_add(lista_de_instrucciones, instruccion);
	}

	fclose(fp);
	if (line) free(line);
}

t_instruccion *crear_instruccion(char **instruccion_leida) {
	id identificador = get_identificador(instruccion_leida[0]);
	if (identificador == -1) return NULL;

	int cant_param = cantidad_de_parametros(identificador); //se podría guardar en instrucción

	t_instruccion *instruccion = (t_instruccion*) malloc(sizeof(t_instruccion));
	instruccion->identificador = identificador;

	if (!cant_param)
		instruccion->parametros = NULL;
	else {
		instruccion->parametros = (uint4_t*) malloc(sizeof(uint4_t) * cant_param);
		for (int i = 0; i < cant_param; i++)
			instruccion->parametros[i] = atoi(instruccion_leida[i + 1]);
	}
	return instruccion;
}

id get_identificador(char* identificador_leido) {
	id identificador = -1;
	if (!strcmp("NO_OP", identificador_leido))	identificador = NO_OP;
	if (!strcmp("I/O", identificador_leido))	identificador = IO;
	if (!strcmp("READ", identificador_leido))	identificador = READ;
	if (!strcmp("COPY", identificador_leido))	identificador = COPY;
	if (!strcmp("WRITE", identificador_leido))	identificador = WRITE;
	if (!strcmp("EXIT", identificador_leido))	identificador = EXIT;
	return identificador;
}

int cantidad_de_parametros(id identificador) {
	switch (identificador) {
	case NO_OP: case IO: case READ: //ver parametro NO_OP
		return 1;
	case COPY: case WRITE:
		return 2;
	case EXIT: default:
		return 0;
	}
}

void leer_lista(t_list *lista_de_instrucciones) {
	if(!list_is_empty(lista_de_instrucciones)) {
		puts("\nLeyendo lista de instrucciones\n------------------------------\n");
		list_iterate(lista_de_instrucciones, leer_instruccion);
	}
}

void leer_instruccion(void *instruccion) {
	id identificador = ((t_instruccion*) instruccion)->identificador;
	uint4_t cant_param = cantidad_de_parametros(identificador);
	uint4_t *parametros = ((t_instruccion*) instruccion)->parametros;

	printf("identificador: %d\nparametros: ", identificador);

	if (!cant_param) puts("no hay parametros\n\n");

	for (int i = 0; i < cant_param; i++) {
		if (i == cant_param - 1) printf("%d\n\n", parametros[i]);
		else printf("%d, ", parametros[i]);
	}
}

void liberar_lista(t_list *lista_de_instrucciones) {
	list_destroy_and_destroy_elements(lista_de_instrucciones, liberar_instruccion);
}

void liberar_instruccion(void *instruccion) {
	free(((t_instruccion*) instruccion)->parametros);
	free((t_instruccion*) instruccion);
}
