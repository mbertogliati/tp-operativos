#include "../include/consola.h"
#include <stdlib.h>
#include <commons/string.h>

void procesar_linea(char **instruccion_leida, t_list *instrucciones) {
	int identificador = get_identificador(instruccion_leida[0]);
	uint32_t *parametros = NULL;
	int i = 0;

	switch (identificador) {
	case NO_OP:
		for (; i < atoi(instruccion_leida[1]); i++)
			list_add(instrucciones, crear_instruccion(identificador, 0, parametros));
		break;

	case IO: case READ: case WRITE: case COPY: case EXIT:
		while (instruccion_leida[i + 1]) {
			parametros = (uint32_t*) realloc(parametros, sizeof(uint32_t) * (i + 1));
			parametros[i] = atoi(instruccion_leida[i + 1]);
			i++;
		}
		list_add(instrucciones, crear_instruccion(identificador, i, parametros));
		break;
	}
}

t_list *leer_archivo(char *path) {
	FILE *f = fopen(path, "r");

	if (!f) {
		log_error(logger, "No se encontró el archivo de instrucciones");
		return NULL;
	}

	char *linea = NULL;
	size_t lon = 0;
	t_list *instrucciones = list_create();

	while (getline(&linea, &lon, f) != -1)
		procesar_linea(string_split(linea, " "), instrucciones);

	fclose(f);
	free(linea);

	log_info(logger, "Archivo de instrucciones leído correctamente");
	log_info(logger, "Lista de instrucciones creada");
	return instrucciones;
}
