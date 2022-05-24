#include "../../include/estructuras/pcb.h"

void imprimir_pcb(t_pcb *pcb) {
	printf("\n\nId: %d\nTamaño: %d\nInstrucciones:\n\n",
			pcb->id,
			pcb->tamanio);
	list_iterate(pcb->instrucciones, (void *) imprimir_instruccion);
}

t_pcb *recibir_paquete_consola(void *buffer, int size) {
	int desplazamiento = 0;
	t_instruccion *instruccion;

	t_pcb *pcb = (t_pcb *) malloc(sizeof(t_pcb));

	memcpy(&(pcb->tamanio), buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	while(desplazamiento < size) {
		instruccion = desempaquetar_instruccionp(&desplazamiento);
	}
}

t_pcb *desempaquetar_pcb(void *buffer) {
	int desplazamiento = 0;
	t_pcb *pcb;
	pcb->instrucciones = list_create();

	memcpy(&(pcb->id), buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(&(pcb->tamanio), buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	pcb->instrucciones = desempaquetar_instrucciones(buffer, &desplazamiento, size);

	memcpy(&(pcb->program_counter), buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(&(pcb->tabla_paginas), buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(&(pcb->est_rafaga), buffer + desplazamiento, sizeof(float));
	desplazamiento += sizeof(float);

	return pcb;
}


void liberar_pcb(t_pcb *pcb) {
	list_destroy_and_destroy_elements(pcb->instrucciones, (void*) liberar_instruccion);
	free(pcb);
}
