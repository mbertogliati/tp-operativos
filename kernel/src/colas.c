#include "../include/colas.h"

void conectar_consola() {
	int socket_servidor = iniciar_servidor(puerto_escucha());
	log_info(logger, "Servidor listo para recibir al cliente");

	// colas
	new = queue_create();

	// semáforos
	sem_init(&grado_multip, 0, grado_multiprogramacion());
	sem_init(&procesos, 0, 0);

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

	pcb->id = 1;
	pcb->program_counter = 0;
	pcb->est_rafaga = grado_multiprogramacion();

	return pcb;
}

// new
void proceso_new(int *socket_cliente) {
	while (1) {
		t_pcb *pcb;
		switch (recibir_operacion(*socket_cliente)) {
		case INSTRUCCIONES_CONSOLA:
			pcb = generar_pcb(*socket_cliente);

			log_info(logger, "Recibí el proceso:\n");
			imprimir_pcb(pcb);

			pthread_mutex_lock(&mnew);
			queue_push(new, pcb);
			pthread_mutex_unlock(&mnew);
			sem_post(&procesos);

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

// ready-fifo
void agregar_a_ready_fifo(t_pcb *pcb, char *algoritmo) {
	sem_wait(&procesos);
	sem_wait(&grado_multip);

	pthread_mutex_lock(&mready);
	list_add(ready, queue_pop(new));
	pthread_mutex_unlock(&mready);
}


// ready-sjf
double calcular_estimacion(double const_alfa, double real, t_pcb *pcb) {
	pcb->est_rafaga = pcb->est_rafaga * const_alfa + real * (1 - const_alfa);
	return pcb->est_rafaga;
}

int menor_estimacion(t_pcb *pcb1, t_pcb * pcb2) {
	return pcb1->est_rafaga < pcb2->est_rafaga;
}

void agregar_a_ready_sjf(t_pcb *pcb, char *algoritmo) {
	sem_wait(&procesos);
	sem_wait(&grado_multip);

	pthread_mutex_lock(&mready);
	list_add_sorted(ready, queue_pop(new), (void *)menor_estimacion);
	pthread_mutex_unlock(&mready);
}
