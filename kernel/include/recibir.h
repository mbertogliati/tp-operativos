#ifndef RECIBIR_H_
#define RECIBIR_H_

#include <string.h>
#include <commons/collections/list.h>
#include "../../shared/include/instrucciones.h"
#include "../../shared/include/server_utils.h"

typedef struct {
	int id;
	int tamanio;
	t_list *instrucciones;
	int program_counter;
	//tabla_paginas;
	//est_rafaga;
} pcb;

pcb *generar_pcb(int socket_cliente);
void imprimir_pcb(pcb *bloque_control);

#endif /* RECIBIR_H_ */
