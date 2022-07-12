#include "../include/conexion_kernel.h"

int conectar_a_kernel() {
	t_config *config = config_create("consola.config");
	char *ip, *puerto;

	if (!config) {
		log_error(logger, "No se encontró el archivo de configuracion de Kernel");
		return -1;
	}

	if (config_has_property(config, "IP_KERNEL") &&
		config_has_property(config, "PUERTO_KERNEL")) {
		ip = config_get_string_value(config, "IP_KERNEL");
		puerto = config_get_string_value(config, "PUERTO_KERNEL");
	}
	else {
		log_error(logger, "El archivo de configuración de Kernel es erróneo");
		return -1;
	}

	log_info(logger, "Iniciando conexión con Kernel");
	int conexion = crear_conexion(ip, puerto);
	config_destroy(config);
	return conexion;
}

bool conexion_exitosa(int socket) {
	if (socket == -1) {
		log_error(logger, "No se ha podido establecer comunicación con el Kernel");
		return false;
	}
	else {
		log_info(logger, "Conexión exitosa!");
		return true;
	}
}

void enviar_paquete_instrucciones(t_paquete *paquete, int socket_kernel) {
	log_info(logger, "Enviando paquete");
	enviar_paquete(paquete, socket_kernel);
	log_info(logger, "Paquete enviado");
}

void terminar(int socket_kernel) {
	int mensaje;
	recv(socket_kernel, &mensaje, sizeof(int), MSG_WAITALL);
	if (mensaje == TERMINAR_CONSOLA) {
		log_info(logger, "Terminando consola");
		liberar_conexion(socket_kernel);
		log_destroy(logger);
		return;
	}
	log_error(logger, "Operación desconocida");
}
