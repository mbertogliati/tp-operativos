#include "../include/crear_paquete.h"

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

t_paquete *crear_paquete_instrucciones(char *path, int tamanio) {
	log_info(logger, "Creando paquete de instrucciones...");

	t_paquete *paquete = crear_paquete(INSTRUCCIONES_CONSOLA);
	agregar_a_paquete(paquete, &tamanio, sizeof(int));

	t_list *instrucciones = leer_archivo(path);
	t_list_iterator *iterador = list_iterator_create(instrucciones);

	while (list_iterator_has_next(iterador)) {
		t_instruccion *instruccion = list_iterator_next(iterador);
		agregar_a_paquete(paquete, &(instruccion->identificador), sizeof(int));
		int cant_parametros = instruccion->cant_parametros;
		agregar_a_paquete(paquete, &cant_parametros, sizeof(int));
		if (cant_parametros)
			agregar_a_paquete(paquete, instruccion->parametros, sizeof(uint32_t) * cant_parametros);
	}

	list_iterator_destroy(iterador);
	list_destroy_and_destroy_elements(instrucciones, (void *) liberar_instruccion);
	log_info(logger, "Paquete creado exitosamente");

	return paquete;
}
