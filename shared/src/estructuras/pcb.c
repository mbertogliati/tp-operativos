#include "../../include/estructuras/pcb.h"
#include "../../include/estructuras/instrucciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimir_pcb(t_pcb *pcb) {
	printf("\n\nId: %d\nTamaño: %d\nCantidad de instrucciones: %d\nInstrucciones:\n\n",
			pcb->id,
			pcb->tamanio,
			pcb->cant_instrucciones);
	list_iterate(pcb->instrucciones, (void *) imprimir_instruccion);
}

void recibir_paquete_consola(void *buffer, int size, t_pcb *pcb) {
	int desplazamiento = 0;

	memcpy(&(pcb->tamanio), buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	pcb->instrucciones = list_create();

	while(desplazamiento < size)
		list_add(pcb->instrucciones, desempaquetar_instruccion(buffer, &desplazamiento));

	pcb->cant_instrucciones = list_size(pcb->instrucciones);
}

t_pcb *desempaquetar_pcb(void *buffer) {
	int desplazamiento = 0;
	t_pcb *pcb = (t_pcb *) malloc(sizeof(t_pcb));
	pcb->instrucciones = list_create();

	memcpy(&(pcb->id), buffer + desplazamiento, sizeof(uint16_t));
	desplazamiento += sizeof(uint16_t);

	memcpy(&(pcb->tamanio), buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(&(pcb->cant_instrucciones), buffer + desplazamiento, sizeof(uint8_t));
	desplazamiento += sizeof(uint8_t);

	for (int i = 0; i < pcb->cant_instrucciones; i++)
		list_add(pcb->instrucciones, desempaquetar_instruccion(buffer, &desplazamiento));

	memcpy(&(pcb->program_counter), buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(&(pcb->tabla_paginas), buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(&(pcb->est_rafaga), buffer + desplazamiento, sizeof(double));
	desplazamiento += sizeof(double);

	return pcb;
}

void liberar_pcb(t_pcb *pcb) {
	list_destroy_and_destroy_elements(pcb->instrucciones, (void*) liberar_instruccion);
	free(pcb);
}
