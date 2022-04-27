#include "../include/server_utils.h"

/******* CONECTAR *******/
int iniciar_servidor(void) {
	struct addrinfo *server_info = addrinfo_servidor(IP, PUERTO);
	int socket_servidor = crear_socket(server_info);

	bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(server_info);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor) {
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conectó un cliente!");
	return socket_cliente;
}

/******* RECIBIR *******/
int recibir_operacion(int socket_cliente) {
	int cod_op;
	if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else {
		close(socket_cliente);
		return -1;
	}
}

void *recibir_buffer(int* size, int socket_cliente) {
	void *buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente) {
	int size;
	char *buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llegó el mensaje %s", buffer);
	free(buffer);
}

t_list *recibir_paquete(int socket_cliente) {
	int size, desplazamiento = 0;
	void *buffer;
	buffer = recibir_buffer(&size, socket_cliente);

	t_list *instrucciones = list_create();
	t_instruccion *instruccion;

	while (desplazamiento < size) {
		int identificador, cant_parametros;
		uint32_t *parametros;

		memcpy(&identificador, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(&cant_parametros, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		if (cant_parametros) {
			int tam_cant_parametros = cant_parametros * sizeof(uint32_t);
			parametros = (uint32_t *) malloc(tam_cant_parametros);

			memcpy(parametros, buffer + desplazamiento, tam_cant_parametros);
			desplazamiento += tam_cant_parametros;
		}
		else parametros = NULL;

		instruccion = crear_instruccion(identificador, cant_parametros, parametros);
		list_add(instrucciones, instruccion);
	}

	free(buffer);
	return instrucciones;
}
