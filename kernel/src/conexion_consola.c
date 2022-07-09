#include "../include/conexion_consola.h"

int kernel;

void conectar_consola(char *puerto) {
	int kernel = iniciar_servidor(puerto);
	log_info(memoria_log, "Servidor listo para recibir al cliente");

	// hilos
	pthread_t thread;

	pid_counter = 0;

	while (1) {
		int consola = esperar_cliente(kernel);
		log_info(memoria_log, "Se conectó un cliente!");
		pthread_create(&thread, NULL, (void *) proceso_new, &consola);
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

			log_info(memoria_log, "Recibí el proceso:\n");
			imprimir_pcb(pcb);

			//Checkpoint
			//Le digo a memoria que me cree el proceso
//			sem_wait(&mutex_memoria);
//			pcb->id = pid_counter;
//			pid_counter++;
//			pcb->tabla_paginas = agregar_proceso_memoria(pcb->id, pcb->tamanio);
//			sem_post(&mutex_memoria);

			//Agrego a la cola de new
//			sem_wait(&mutex_new);
//			pcb->program_counter = 0;
//			queue_push(queue_new, pcb);
//			sem_post(&mutex_new);
			//agregar_a_new(pcb);

//			sem_post(&procesos_en_new);

			agregar_a_new(pcb);

			break;

		case -1:
			log_info(memoria_log, "El cliente se desconectó.");
			return;

		default:
			log_error(memoria_log, "Código de operación inválido.");
			return;
		}
	}
}

void terminar_consola(int socket_consola) {
	const int TERMINAR = 0;
	send(socket_consola, (void *) TERMINAR, sizeof(int), 0);
}
