#include "../include/crear_paquete.h"

void empaquetar_instruccion(t_paquete *paquete, t_instruccion *instruccion) {
	if (!instruccion)
		return;

	if (instruccion->identificador == NO_OP) {
		if (instruccion->parametros[0] > 1) {
			instruccion->parametros[0]--;
			empaquetar_instruccion(paquete, instruccion);
		}
		instruccion = crear_instruccion(NO_OP, 0, NULL);
	}

	agregar_a_paquete(paquete, &(instruccion->identificador), sizeof(int));
	agregar_a_paquete(paquete, &(instruccion->cant_parametros), sizeof(int));
	agregar_a_paquete(paquete, instruccion->parametros, sizeof(uint32_t) * instruccion->cant_parametros);
}

t_instruccion *procesar_linea(char *id, char **param) {
	int identificador = get_identificador(id);
	int cant_parametros = 0;
	uint32_t *parametros = NULL;

	switch (identificador) {
	case EXIT:
		break;

	case NO_OP: case IO: case READ:
		cant_parametros = 1;
		break;

	case WRITE: case COPY:
		cant_parametros = 2;
		break;

	default:
		log_error(logger, "Identificador no reconocido");
		return NULL;
	}

	if (cant_parametros) {
		parametros = (uint32_t *) malloc(cant_parametros * sizeof(uint32_t));
		for (int i = 0; i < cant_parametros; i++)
			parametros[i] = atoi(param[i]);
	}

	return crear_instruccion(identificador, cant_parametros, parametros);
}

t_paquete *crear_paquete_instrucciones(char *path, int tamanio) {
	FILE *f = fopen(path, "r");

	if (!f) {
		log_error(logger, "No se encontró el archivo de instrucciones");
		return NULL;
	}

	char *linea = NULL;
	size_t lon = 0;
	char **instruccion;

	log_info(logger, "Creando paquete de instrucciones...");
	t_paquete *paquete = crear_paquete(INSTRUCCIONES_CONSOLA);
	agregar_a_paquete(paquete, &tamanio, sizeof(int));

	while (getline(&linea, &lon, f) != -1) {
		instruccion = string_split(linea, " ");
		empaquetar_instruccion(
				paquete,
				procesar_linea(instruccion[0], instruccion + sizeof(char *))
		);
	}

	fclose(f);
	free(linea);

	log_info(logger, "Archivo de instrucciones leído correctamente");
	log_info(logger, "Paquete creado exitosamente");
	return paquete;
}

/*
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
*/
