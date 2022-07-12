#ifndef PCB_H_
#define PCB_H_

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct {
	uint16_t id;
	uint32_t tamanio;
	uint8_t cant_instrucciones;
	t_list *instrucciones;
	uint32_t program_counter;
	int tabla_paginas;
	double est_rafaga;
} t_pcb;

void imprimir_pcb(t_pcb *pcb);
int enviar_pcb(t_pcb* pcb, int socket_a_enviar, int IO);
t_pcb *desempaquetar_pcb(void *buffer);
void liberar_pcb(t_pcb *pcb);

#endif /* PCB_H_ */
