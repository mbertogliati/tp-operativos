#include "../include/client_utils.h"

/******* CONECTAR *******/
int crear_conexion(char *ip, char *puerto) {
	struct addrinfo *server_info = addrinfo_servidor(ip, puerto);
	int socket_cliente = crear_socket(server_info);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		return -1;

	freeaddrinfo(server_info);
	return socket_cliente;
}

void liberar_conexion(int socket_cliente) {
	close(socket_cliente);
}

/******* SERIALIZAR *******/
void *serializar_paquete(t_paquete *paquete, int bytes) {
	void *magic = malloc(bytes);
	int desplazamiento = 0;
	t_buffer *buffer = paquete->buffer;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &(buffer->size), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, buffer->stream, buffer->size);
	desplazamiento += buffer->size;

	return magic;
}

/******* MENSAJE *******/
void enviar_mensaje(char *mensaje, int socket_cliente) {
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2 * sizeof(int);

	void *a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

/******* PAQUETE *******/
void crear_buffer(t_paquete* paquete) {
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete *crear_paquete(void) {
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete *paquete, void *valor, int bytes) {
	t_buffer *buffer = paquete->buffer;

	buffer->stream = realloc(buffer->stream, buffer->size + bytes);
	memcpy(buffer->stream + buffer->size, valor, bytes);
	buffer->size += bytes;
}

void enviar_paquete(t_paquete* paquete, int socket_cliente) {
	int bytes = paquete->buffer->size + 2 * sizeof(int);
	void *a_enviar = serializar_paquete(paquete, bytes);
	send(socket_cliente, a_enviar, bytes, 0);
	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
