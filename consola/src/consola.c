#include "../include/consola.h"
#include <stdlib.h>

int main(int argc, char **argv) {
	if(!son_argumentos_validos(argc, argv))
		return EXIT_FAILURE;

	consola(argv[1], atoi(argv[2]));

	return EXIT_SUCCESS;
}

bool son_argumentos_validos(int cantidad_argumentos, char **argv) {
	logger = log_create("consola.log", "CONSOLA", true, LOG_LEVEL_INFO);

	if (cantidad_argumentos < 3) {
		cantidad_argumentos < 2 ?
				log_error(logger, "Archivo no especificado.\n") :
				log_error(logger, "Tamaño no especificado.\n");
		return false;
	}

	if (atoi(argv[2]) <= 0) {
		log_error(logger, "El tamaño es inválido.\n");
		return false;
	}

	return true;
}

void consola(char *path, int tamanio) {
	t_list *instrucciones = leer_archivo(path);
	t_paquete *paquete = crear_paquete_instrucciones(instrucciones, tamanio);
	enviar_paquete_instrucciones(paquete);
	liberar_lista(instrucciones);
	log_destroy(logger);
}
