#include "../include/client_utils.h"

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

int crear_conexion(char *ip, char *puerto) {
	struct addrinfo *server_info = addrinfo_servidor(ip, puerto);
	int socket_cliente = crear_socket(server_info);

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen)
			== -1)
		return -1;

	freeaddrinfo(server_info);
	return socket_cliente;
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
}

void enviar_mensaje(char* mensaje, int socket_cliente) {
	t_paquete *paquete = crear_paquete(MENSAJE);
	agregar_a_paquete(paquete, mensaje, strlen(mensaje) + 1);
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
