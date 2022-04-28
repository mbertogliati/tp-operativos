#include "../include/client_utils.h"

int crear_conexion(char *ip, char *puerto) {
	struct addrinfo *server_info = addrinfo_servidor(ip, puerto);
	int socket_cliente = crear_socket(server_info);

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		return -1;

	freeaddrinfo(server_info);
	return socket_cliente;
}

void liberar_conexion(int socket_cliente) {
	close(socket_cliente);
}

/*
 Cambie la variable bytes por size_serializado y ademas ahora la variable se sobreescribe al terminar la funcion
 dando el tamaño correcto del buffer a enviar

void *serializar_paquete(t_paquete *paquete, int *size_serializado) {
	void *magic = NULL;
	int desplazamiento = 0;
	t_buffer *buffer = paquete->buffer;

	magic = realloc(magic, sizeof(int) + desplazamiento);
	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);

	magic = realloc(magic, sizeof(int) + desplazamiento);
	memcpy(magic + desplazamiento, &(buffer->size), sizeof(int));
	desplazamiento += sizeof(int);

	magic = realloc(magic, buffer->size + desplazamiento);
	memcpy(magic + desplazamiento, buffer->stream, buffer->size);
	desplazamiento += buffer->size;

	*size_serializado = desplazamiento;
	return magic;
}

void enviar_paquete(t_paquete* paquete, int socket_cliente) {
	int size_serializado;
	void *a_enviar = serializar_paquete(paquete, &size_serializado);
	send(socket_cliente, a_enviar, size_serializado, 0);
	free(a_enviar);
}

*/

void agregar_a_paquete(t_paquete *paquete, void *valor, int bytes) {
	t_buffer *buffer = paquete->buffer;
	// se agranda el tamaño del buffer
	buffer->stream = realloc(buffer->stream, buffer->size + bytes);
	// se guarda el valor en el buffer
	memcpy(buffer->stream + buffer->size, valor, bytes);
	// se actualiza el tam del buffer
	buffer->size += bytes;
}

void enviar_paquete_original(t_paquete* paquete, int socket_cliente)
{
	int size_serializado = paquete->buffer->size + 2 * sizeof(int);
	/*
	 * size_serializado guarda el tamaño del buffer más el tamaño de cod_op
	 * más el espacio donde se guarda el tamaño del buffer
	 */
	void* a_enviar = serializar_paquete_original(paquete, size_serializado);
	send(socket_cliente, a_enviar, size_serializado, 0);
	free(a_enviar);
}

void *serializar_paquete_original(t_paquete *paquete, int size_serializado) {
	void *magic = malloc(size_serializado); //entonces acá ya tiene el tamaño total
	int desplazamiento = 0;
	t_buffer *buffer = paquete->buffer;
	// se guarda el cod_op
	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);
	// se guarda el tam del buffer
	memcpy(magic + desplazamiento, &(buffer->size), sizeof(int));
	desplazamiento += sizeof(int);
	// se guarda el buffer
	memcpy(magic + desplazamiento, buffer->stream, buffer->size);

	return magic;
}

void crear_buffer(t_paquete* paquete) {
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete *crear_paquete(op_code codigo_operacion) {
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo_operacion;
	crear_buffer(paquete);
	return paquete;
}

void eliminar_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
