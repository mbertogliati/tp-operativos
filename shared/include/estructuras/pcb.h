#ifndef PCB_H_
#define PCB_H_

#include <commons/collections/list.h>

typedef struct {
	int id;
	int tamanio;
	int cant_instrucciones;
	t_list *instrucciones;
	int program_counter;
	int tabla_paginas;
	float est_rafaga;
} t_pcb;

void imprimir_pcb(t_pcb *pcb);
void recibir_paquete_consola(void *buffer, int size, t_pcb *pcb);
t_pcb *desempaquetar_pcb(void *buffer);
void liberar_pcb(t_pcb *pcb);

#endif /* PCB_H_ */
