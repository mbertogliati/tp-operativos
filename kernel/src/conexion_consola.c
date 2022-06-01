#include "../include/conexion_consola.h"

void conectar_consola(char *puerto) {
	int socket_servidor = iniciar_servidor(puerto);
	log_info(logger, "Servidor listo para recibir al cliente");

	// hilos
	pthread_t thread;

	while (1) {
		int socket_cliente = esperar_cliente(socket_servidor);
		log_info(logger, "Se conectó un cliente!");
		pthread_create(&thread, NULL, (void *) proceso_new, &socket_cliente);
		pthread_detach(thread);
	}
}

t_pcb *generar_pcb(int socket_cliente) {
	int size;
	void *buffer = recibir_buffer(&size, socket_cliente);
	t_pcb *pcb = (t_pcb *) malloc(sizeof(t_pcb));
	recibir_paquete_consola(buffer, size, pcb);

	return pcb;
}

void proceso_new(int *socket_cliente) {
	while (1) {
		t_pcb *pcb;
		switch (recibir_operacion(*socket_cliente)) {
		case INSTRUCCIONES_CONSOLA:
			pcb = generar_pcb(*socket_cliente);

			log_info(logger, "Recibí el proceso:\n");
			imprimir_pcb(pcb);
			agregar_a_new(pcb);

			break;

		case -1:
			log_info(logger, "El cliente se desconectó.");
			return;

		default:
			log_error(logger, "Código de operación inválido.");
			return;
		}
	}
}
