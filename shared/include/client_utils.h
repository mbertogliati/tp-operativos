#ifndef CLIENT_UTILS_H_
#define CLIENT_UTILS_H_

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

t_paquete *crear_paquete(op_code codigo_operacion);
void agregar_a_paquete(t_paquete *paquete, void *valor, int bytes);
int crear_conexion(char *ip, char *puerto);
void enviar_paquete(t_paquete *paquete, int socket_cliente);
void enviar_mensaje(char* mensaje, int socket_cliente);
void eliminar_paquete(t_paquete *paquete);
void liberar_conexion(int socket_cliente);

#endif /* CLIENT_UTILS_H_ */