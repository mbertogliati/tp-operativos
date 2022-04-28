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

void *recibir_buffer(int *size, int socket_cliente) {
	void *buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}
/*
	nueva funcion que toma el valor del buffer en ese momento y lo castea a int 
	(que es supuestamente donde esta el argumento "tamanio")
*/
int recibir_tamanio_en_memoria(void *buffer){ 
	int *memory_size;
	memory_size = buffer;
	return (*memory_size);
}

void recibir_mensaje(int socket_cliente) {
	int size;
	int *buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llegó el mensaje %d", *buffer);
	free(buffer);
}

