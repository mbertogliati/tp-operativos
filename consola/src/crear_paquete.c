#include "../include/crear_paquete.h"

void empaquetar_instruccion(t_paquete *paquete, t_instruccion *instruccion) {
	if (!instruccion)
		return;

	if (instruccion->identificador == NO_OP) {
		if (instruccion->parametros[0] > 1) {
			instruccion->parametros[0]--;
			empaquetar_instruccion(paquete, instruccion);
		}
		instruccion->cant_parametros = 0;
		instruccion->parametros = NULL;
	}

	agregar_a_paquete(paquete, &(instruccion->identificador), sizeof(int));
	agregar_a_paquete(paquete, &(instruccion->cant_parametros), sizeof(int));
	agregar_a_paquete(paquete, instruccion->parametros, sizeof(uint32_t) * instruccion->cant_parametros);
}

t_instruccion *procesar_linea(char *id, char **param) {
	int identificador = get_identificador(id);
	if (identificador == -1) {
		log_error(logger, "Identificador \"%s\" no reconocido", id);
		return NULL;
	}

	int cant_parametros = get_cant_parametros(identificador);
	uint32_t *parametros;

	if (cant_parametros) {
		parametros = (uint32_t *) malloc(cant_parametros * sizeof(uint32_t));
		for (int i = 0; i < cant_parametros; i++)
			parametros[i] = atoi(param[i]);
	}
	else parametros = NULL;

	return crear_instruccion(identificador, cant_parametros, parametros);
}

t_paquete *crear_paquete_instrucciones(char *path, int tamanio) {
	FILE *f = fopen(path, "r");

	if (!f) {
		log_error(logger, "No se encontró el archivo de instrucciones");
		return NULL;
	}

	log_info(logger, "Creando paquete de instrucciones...");
	t_paquete *paquete = crear_paquete(INSTRUCCIONES_CONSOLA);
	agregar_a_paquete(paquete, &tamanio, sizeof(int));

	char *linea = NULL;
	size_t lon = 0;
	char **instruccion;

	while (getline(&linea, &lon, f) != -1) {
		instruccion = string_split(linea, " ");
		empaquetar_instruccion(
				paquete,
				procesar_linea(instruccion[0], instruccion + 1)
		);

	}

	fclose(f);
	free(linea);

	log_info(logger, "Archivo de instrucciones leído correctamente");
	log_info(logger, "Paquete creado exitosamente");
	return paquete;
}
