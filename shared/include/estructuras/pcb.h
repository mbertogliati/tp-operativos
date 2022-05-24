#ifndef PCB_H_
#define PCB_H_

#include "instrucciones.h"
#include <commons/collections/list.h>

typedef struct {
	int id;
	int tamanio;
	t_list *instrucciones;
	int program_counter;
	int tabla_paginas;
	float est_rafaga;
} t_pcb;

void imprimir_pcb(t_pcb *pcb);
t_pcb *desempaquetar_pcb(void *buffer);

#endif /* PCB_H_ */
