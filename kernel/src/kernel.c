#include "../include/kernel.h"

int main() {
	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);

	int socket_servidor = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int socket_cliente = esperar_cliente(socket_servidor);

	//t_paquete_consola* paquete_consola;
	while (1) {
		switch (recibir_operacion(socket_cliente)) {
		case INSTRUCCIONES_CONSOLA:
			log_info(logger, "Se conectó un cliente!");
			/*
			paquete_consola = recibir_paquete_instrucciones_consola(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			leer_lista(paquete_consola->lista_de_instrucciones);

			log_info(logger, "El proceso ocupara:\n");
			printf("%d Bytes de Memoria\n", paquete_consola->memory_size);

			liberar_lista(paquete_consola->lista_de_instrucciones);
			free(paquete_consola);
			*/
			recibir_proceso(socket_cliente);
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

// Ejemplo de cómo se recibiría un proceso + implementación de las funciones que agregué

void recibir_proceso(int socket_cliente) {
	int tamanio;
	t_list *instrucciones = list_create();
	recibir_proceso_consola(socket_cliente, &tamanio, instrucciones);

	pcb bloque_control = {
			.id = 1234,
			.tamanio = tamanio,
			.instrucciones = instrucciones,
			.program_counter = 0
	};

	log_info(logger, "Recibí el proceso:\n");
	printf("\n\nId: %d\nTamaño: %d\nInstrucciones:\n\n",
			bloque_control.id, bloque_control.tamanio);
	imprimir_lista(bloque_control.instrucciones);
	liberar_lista(bloque_control.instrucciones);
}
