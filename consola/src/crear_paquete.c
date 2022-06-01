#include "../include/crear_paquete.h"

void empaquetar_instruccion(t_paquete *paquete, char **instruccion) {
	uint8_t identificador = get_identificador(instruccion[0]);

	if (identificador == NO_RECONOCIDO) {
		log_error(logger, "Identificador \"%s\" no reconocido", instruccion[0]);
		return;
	}

	uint8_t cant_parametros = get_cant_parametros(identificador);

	if (identificador == NO_OP)
		for (int i = 0; i < atoi(instruccion[1]); i++) {
			agregar_a_paquete(paquete, &(identificador), sizeof(uint8_t));
			agregar_a_paquete(paquete, &(cant_parametros), sizeof(uint8_t));
		}

	else {
		agregar_a_paquete(paquete, &(identificador), sizeof(uint8_t));
		agregar_a_paquete(paquete, &(cant_parametros), sizeof(uint8_t));

		if (cant_parametros)
			for (int i = 1, parametro; i < cant_parametros + 1; i++) {
				parametro = atoi(instruccion[i]);
				agregar_a_paquete(paquete, &(parametro), sizeof(uint32_t));
			}
	}
}

t_paquete *crear_paquete_instrucciones(char *path, uint32_t tamanio) {
	FILE *f = fopen(path, "r");

	if (!f) {
		log_error(logger, "No se encontró el archivo de instrucciones");
		return NULL;
	}

	log_info(logger, "Creando paquete de instrucciones...");
	t_paquete *paquete = crear_paquete(INSTRUCCIONES_CONSOLA);

	agregar_a_paquete(paquete, &tamanio, sizeof(uint32_t));

	char *linea = NULL;
	size_t lon = 0;

	while (getline(&linea, &lon, f) != -1)
		empaquetar_instruccion(paquete, string_split(linea, " "));

	fclose(f);
	free(linea);

	log_info(logger, "Archivo de instrucciones leído correctamente");
	log_info(logger, "Paquete creado exitosamente");
	return paquete;
}
