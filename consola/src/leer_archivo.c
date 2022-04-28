#include "../include/leer_archivo.h"

void procesar_linea(char **instruccion_leida, t_list *instrucciones) {
	int identificador = get_identificador(instruccion_leida[0]);
	int cant_parametros = 0;
	uint32_t *parametros = NULL;

	switch (identificador) {
	case NO_OP:
		for (int i = 0; i < atoi(instruccion_leida[1]); i++)
			list_add(instrucciones, crear_instruccion(identificador, cant_parametros, parametros));
		return;

	case IO: case READ:
		cant_parametros = 1;
		break;

	case WRITE: case COPY:
		cant_parametros = 2;
		break;

	case EXIT:
		break;

	default:
		// identificador no reconocido
		return;
	}

	if (cant_parametros) {
		parametros = (uint32_t *) malloc(cant_parametros * sizeof(uint32_t));

		for (int i = 0; i < cant_parametros; i++)
			parametros[i] = atoi(instruccion_leida[i + 1]);
	}

	list_add(instrucciones, crear_instruccion(identificador, cant_parametros, parametros));
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
