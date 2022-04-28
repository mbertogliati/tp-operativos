#ifndef KERNEL_H_
#define KERNEL_H_

#include <commons/log.h>
#include "../../shared/include/instrucciones.h"
#include "../../shared/include/server_utils.h"

typedef struct {
	int memory_size;
	t_list *lista_de_instrucciones;
} t_paquete_consola;

typedef struct {
	int id;
	int tamanio;
	t_list *instrucciones;
	int program_counter;
	//tabla_paginas;
	//est_rafaga;
} pcb;

t_log *logger;

void recibir_proceso(int);
void recibir_proceso_consola(int, int *, t_list *);
t_paquete_consola *recibir_paquete_instrucciones_consola(int);

#endif /* KERNEL_H_ */
