#include "../include/colas.h"

void inicializar_colas() {
	new = queue_create();
	ready = list_create();

	sem_init(&grado_multip, 0, grado_multiprogramacion());
	sem_init(&procesos, 0, 0);
}

// new
void agregar_a_new(t_pcb *pcb) {
	pcb->id = 1;
	pcb->program_counter = 0;
	pcb->est_rafaga = estimacion_inicial();

	pthread_mutex_lock(&mnew);
	queue_push(new, pcb);
	pthread_mutex_unlock(&mnew);

	sem_post(&procesos);

	agregar_a_ready(pcb);
}

// ready-fifo
void agregar_a_ready_fifo(t_pcb *pcb) {
	sem_wait(&procesos);
	sem_wait(&grado_multip);

	pthread_mutex_lock(&mready);
	list_add(ready, queue_pop(new));
	pthread_mutex_unlock(&mready);
}

// ready-sjf
double calcular_estimacion(double real, t_pcb *pcb) {
	pcb->est_rafaga = pcb->est_rafaga * alfa() + real * (1 - alfa());
	return pcb->est_rafaga;
}

int menor_estimacion(t_pcb *pcb1, t_pcb * pcb2) {
	return pcb1->est_rafaga < pcb2->est_rafaga;
}

void agregar_a_ready_sjf(t_pcb *pcb) {
	sem_wait(&procesos);
	sem_wait(&grado_multip);

	pthread_mutex_lock(&mready);
	list_add_sorted(ready, queue_pop(new), (void *) menor_estimacion);
	pthread_mutex_unlock(&mready);
}

// ready
void agregar_a_ready(t_pcb *pcb) {
	enviar_mensaje_memoria("init", 5);
	pcb->tabla_paginas = (int) recibir_mensaje_memoria();
	if(!strcmp("FIFO", algoritmo_planificacion()))
		agregar_a_ready_fifo(pcb);
	if(!strcmp("SJF", algoritmo_planificacion()))
		agregar_a_ready_sjf(pcb);
}
