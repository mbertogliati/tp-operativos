#include "../include/conexion_memoria.h"

void conectar_memoria(char *ip, char *puerto) {
	log_info(logger, "Conectando a memoria");
	socket_memoria = crear_conexion(ip, puerto);
}

void enviar_mensaje_memoria(void *mensaje, int tam) {
	enviar_mensaje(mensaje, tam, socket_memoria);
}

void *recibir_mensaje_memoria() {
	return recibir_mensaje(socket_memoria);
}

void desconectar_memoria() {
	liberar_conexion(socket_memoria);
}
