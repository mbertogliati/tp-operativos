#include "../include/sockets.h"

struct addrinfo *addrinfo_servidor(char *ip, char *puerto) {
	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	return server_info;
}

int crear_socket(struct addrinfo *server_info) {
	return socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
}

// servidor

int iniciar_servidor(char *puerto) {
	struct addrinfo *server_info = addrinfo_servidor(NULL, puerto);
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

void *recibir_mensaje(int socket_cliente) {
	int size;
	return recibir_buffer(&size, socket_cliente);
}

// cliente

int crear_conexion(char *ip, char *puerto) {
	struct addrinfo *server_info = addrinfo_servidor(ip, puerto);
	int socket_cliente = crear_socket(server_info);

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		return -1;

	freeaddrinfo(server_info);
	return socket_cliente;
}

void crear_buffer(t_paquete *paquete) {
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete *crear_paquete(int codigo_operacion) {
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo_operacion;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete *paquete, void *valor, int bytes) {
	t_buffer *buffer = paquete->buffer;
	buffer->stream = realloc(buffer->stream, buffer->size + bytes);
	memcpy(buffer->stream + buffer->size, valor, bytes);
	buffer->size += bytes;
}

void *serializar_paquete(t_paquete *paquete, int size_serializado) {
	void *magic = malloc(size_serializado);
	int desplazamiento = 0;
	t_buffer *buffer = paquete->buffer;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &(buffer->size), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, buffer->stream, buffer->size);

	return magic;
}

void enviar_paquete(t_paquete* paquete, int socket_cliente) {
	int size_serializado = paquete->buffer->size + 2 * sizeof(int);
	void *a_enviar = serializar_paquete(paquete, size_serializado);
	send(socket_cliente, a_enviar, size_serializado, 0);
	free(a_enviar);
	eliminar_paquete(paquete);
}

void enviar_mensaje(void *mensaje, int bytes, int socket_cliente) {
	t_paquete *paquete = crear_paquete(MENSAJE);
	agregar_a_paquete(paquete, mensaje, bytes);
	enviar_paquete(paquete, socket_cliente);
	eliminar_paquete(paquete);
}

void eliminar_paquete(t_paquete *paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente) {
	close(socket_cliente);
}

void* sacar_de_buffer(t_buffer* buffer, int tam_dato) {
	void* buffer_nuevo = malloc(buffer->size-tam_dato);
	void* dato = malloc(tam_dato);

	memcpy(dato, buffer->stream, tam_dato);
	memcpy(buffer_nuevo, buffer->stream+tam_dato, (buffer->size-tam_dato));

	free(buffer->stream);

	buffer->size -= tam_dato;
	buffer -> stream = buffer_nuevo;

	return dato;
}
