#include "../../shared/include/server_utils.h"

int main() {
	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

	t_list* lista;

	while (1) {
		int cod_op = recibir_operacion(cliente_fd);

		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;

		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			leer_lista(lista);
			break;

		case -1:
			log_error(logger, "El cliente se desconectó. Terminando servidor");
			return EXIT_FAILURE;

		default:
			log_warning(logger, "Operación desconocida.");
			break;
		}
	}

	liberar_lista(lista);
	log_destroy(logger);
	return EXIT_SUCCESS;
}
