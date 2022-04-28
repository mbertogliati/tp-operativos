#ifndef CLIENT_UTILS_H_
#define CLIENT_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "socket.h"

typedef struct {
	int size;
	void *stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer *buffer;
} t_paquete;

int crear_conexion(char *ip, char *puerto);
void liberar_conexion(int socket_cliente);

t_paquete *crear_paquete(op_code codigo_operacion);
void agregar_a_paquete(t_paquete *paquete, void *valor, int bytes);
void eliminar_paquete(t_paquete *paquete);

void *serializar_paquete_original(t_paquete *paquete, int size_serializado);
void enviar_paquete_original(t_paquete *paquete, int socket_cliente);

void *serializar_paquete(t_paquete *paquete, int *size_serializado);
void enviar_paquete(t_paquete *paquete, int socket_cliente);

#endif /* CLIENT_UTILS_H_ */
