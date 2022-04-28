#include "../include/kernel.h"
#include <string.h>

void recibir_proceso_consola(int socket_cliente, int *tamanio, t_list *instrucciones) {
	int size, desplazamiento = 0;
	void *buffer = recibir_buffer(&size, socket_cliente);

	memcpy(tamanio, buffer, sizeof(int));
	desplazamiento += sizeof(int);

	int identificador, cant_parametros, tam_parametros;
	uint32_t *parametros;

	while (desplazamiento < size) {
		memcpy(&identificador, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(&cant_parametros, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		if((tam_parametros = cant_parametros * sizeof(uint32_t)))
			parametros = (uint32_t *) malloc(tam_parametros);
		else
			parametros = NULL;

		memcpy(parametros, buffer + desplazamiento, tam_parametros);
		desplazamiento += tam_parametros;

		list_add(instrucciones,
				crear_instruccion(identificador, cant_parametros, parametros));
	}

	free(buffer);
}

int recibir_tamanio_en_memoria(void *buffer) {
	int *memory_size;
	memory_size = buffer;
	return (*memory_size);
}

t_paquete_consola *recibir_paquete_instrucciones_consola(int socket_cliente) {
	int size, desplazamiento = 0;
	void *buffer = recibir_buffer(&size, socket_cliente);

	int tamanio_en_memoria = recibir_tamanio_en_memoria(buffer);
	desplazamiento += sizeof(int);

	t_list *instrucciones = list_create();
	int identificador, cant_parametros, tam_parametros;
	uint32_t *parametros;

	while (desplazamiento < size) {
		memcpy(&identificador, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(&cant_parametros, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		if((tam_parametros = cant_parametros * sizeof(uint32_t)))
			parametros = (uint32_t *) malloc(tam_parametros);
		else
			parametros = NULL;

		memcpy(parametros, buffer + desplazamiento, tam_parametros);
		desplazamiento += tam_parametros;

		list_add(instrucciones,
				crear_instruccion(identificador, cant_parametros, parametros));
	}

	free(buffer);

	t_paquete_consola *paquete_recibido = malloc(sizeof(t_paquete_consola));
	paquete_recibido->memory_size = tamanio_en_memoria;
	paquete_recibido->lista_de_instrucciones = instrucciones;

	return paquete_recibido;
}
