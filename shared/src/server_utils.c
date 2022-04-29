#include "../include/server_utils.h"

/******* CONECTAR *******/
int iniciar_servidor(void) {
	struct addrinfo *server_info = addrinfo_servidor(IP, PUERTO);
	int socket_servidor = crear_socket(server_info);

	bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(server_info);
	return socket_servidor;
}

int esperar_cliente(int socket_servidor) {
	int socket_cliente = accept(socket_servidor, NULL, NULL);
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

void *recibir_buffer(int *size, int socket_cliente) {
	void *buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

char *recibir_mensaje(int socket_cliente) {
	int size;
	return (char *) recibir_buffer(&size, socket_cliente);
}
