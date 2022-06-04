#ifndef CONEXION_MEMORIA_H_
#define CONEXION_MEMORIA_H_

#include <commons/log.h>

#include <sockets/client_utils.h>
#include <sockets/server_utils.h>

int socket_memoria;
extern t_log *logger;


void conectar_memoria(char *ip, char *puerto);
void enviar_mensaje_memoria(void *mensaje, int tam);
void *recibir_mensaje_memoria();
void desconectar_memoria();

int agregar_proceso_memoria(int pid, int tam_proceso);
bool suspender_proceso_memoria(int direccion_tabla);
bool finalizar_proceso_memoria(int direccion_tabla);

#endif /* CONEXION_MEMORIA_H_ */
