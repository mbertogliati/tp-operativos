#include "../include/kernel.h"

int main() {
	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);

	int socket_servidor = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int socket_cliente = esperar_cliente(socket_servidor);

	pcb *bloque_control;
	char *mensaje;

	while (1) {
		switch (recibir_operacion(socket_cliente)) {
		case INSTRUCCIONES_CONSOLA:
			log_info(logger, "Se conectó un cliente!");
			bloque_control = generar_pcb(socket_cliente);
			log_info(logger, "Recibí el proceso:\n");
			imprimir_pcb(bloque_control);
			liberar_lista(bloque_control->instrucciones);
			break;

		case MENSAJE:
			mensaje = recibir_mensaje(socket_cliente);
			log_info(logger, "Me llegó el mensaje %s", mensaje);
			free(mensaje);
			break;

		case -1:
			log_error(logger, "El cliente se desconectó. Terminando servidor");
			return EXIT_FAILURE;

		default:
			log_warning(logger,"Operación desconocida");
			break;
		}
	}

	log_destroy(logger);
	return EXIT_SUCCESS;
}
