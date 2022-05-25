#include "../include/kernel.h"

int main() {
	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);
	kernel_config = config_create("kernel.config");
	int socket_servidor = iniciar_servidor(puerto_escucha());
	log_info(logger, "Servidor listo para recibir al cliente");
	int socket_cliente = esperar_cliente(socket_servidor);

	t_pcb *pcb = (t_pcb *) malloc(sizeof(t_pcb));
	int size;
	char *buffer;

	while (1) {
		switch (recibir_operacion(socket_cliente)) {
		case INSTRUCCIONES_CONSOLA:
			log_info(logger, "Se conectó un cliente!");
			pcb->id = 1;
			buffer = recibir_buffer(&size, socket_cliente);
			recibir_paquete_consola(buffer, size, pcb);
			log_info(logger, "Recibí el proceso:\n");
			imprimir_pcb(pcb);
			liberar_pcb(pcb);
			break;

		case MENSAJE:
			buffer = (char *) recibir_mensaje(socket_cliente);
			log_info(logger, "Me llegó el mensaje: %s", buffer);
			free(buffer);
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
	config_destroy(kernel_config);
	return EXIT_SUCCESS;
}
