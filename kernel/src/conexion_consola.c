#include "../include/conexion_consola.h"

void conectar_consola(char *puerto) {
	int socket_servidor = iniciar_servidor(puerto);
	log_protegido("CONSOLA:Servidor listo para recibir al cliente");

	// hilos
	pthread_t thread;

	while (1) {
		int socket_cliente = esperar_cliente(socket_servidor);
		log_protegido("CONSOLA:Se conectó un cliente!");
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

			log_protegido("CONSOLA:Recibí el proceso:");
			//imprimir_pcb(pcb);

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
			log_protegido("CONSOLAEl cliente se desconectó.");
			return;

		default:
			log_protegido("CONSOLA_ERROR:Código de operación inválido.");
			return;
		}
	}
}
