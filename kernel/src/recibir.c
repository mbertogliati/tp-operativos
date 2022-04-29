#include "../include/recibir.h"

t_list *desempaquetar_instrucciones(void *buffer, int size) {
	int desplazamiento = 0;

	int identificador, cant_parametros, tam_parametros;
	uint32_t *parametros;
	t_list *instrucciones = list_create();

	while (desplazamiento < size) {
		memcpy(&identificador, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(&cant_parametros, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		if ((tam_parametros = cant_parametros * sizeof(uint32_t)))
			parametros = (uint32_t *) malloc(tam_parametros);
		else
			parametros = NULL;

		memcpy(parametros, buffer + desplazamiento, tam_parametros);
		desplazamiento += tam_parametros;

		list_add(instrucciones,
				crear_instruccion(identificador, cant_parametros, parametros));
	}

	return instrucciones;
}

t_list *recibir_proceso(int socket_cliente, int *tamanio) {
	int size, desplazamiento = 0;
	void *buffer = recibir_buffer(&size, socket_cliente);

	memcpy(tamanio, buffer, sizeof(int));
	desplazamiento += sizeof(int);

	t_list *instrucciones =
			desempaquetar_instrucciones(buffer + desplazamiento, size - desplazamiento);

	free(buffer);
	return instrucciones;
}

pcb *generar_pcb(int socket_cliente) {
	int tamanio;
	t_list *instrucciones =
			recibir_proceso(socket_cliente, &tamanio);

	pcb *bloque_control = (pcb *) malloc(sizeof(pcb));

	bloque_control->id = 1234;
	bloque_control->tamanio = tamanio;
	bloque_control->instrucciones = instrucciones;
	bloque_control->program_counter = 0;

	return bloque_control;
}

void imprimir_pcb(pcb *bloque_control) {
	printf("\n\nId: %d\nTamaño: %d\nInstrucciones:\n\n",
			bloque_control->id,
			bloque_control->tamanio);
	imprimir_lista(bloque_control->instrucciones);
}
