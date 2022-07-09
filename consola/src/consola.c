#include "../include/consola.h"

int main(int argc, char **argv) {
	if(!son_argumentos_validos(argc, argv)) return EXIT_FAILURE;
	t_paquete *paquete = crear_paquete_instrucciones(argv[1], atoi(argv[2]));
	int socket_kernel = enviar_paquete_instrucciones(paquete);
	if(socket_kernel == -1) return EXIT_FAILURE;
	terminar(socket_kernel);
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
