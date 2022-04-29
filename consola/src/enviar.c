#include "../include/enviar.h"

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

	int conexion = crear_conexion(ip, puerto);
	config_destroy(config);

	return conexion;
}

void enviar_paquete_instrucciones(t_paquete *paquete) {
	log_info(logger, "Iniciando conexión con Kernel...");
	int socket_cliente = conectar_a_kernel();

	if (socket_cliente == -1) {
		log_error(logger, "No se ha podido establecer comunicación con el Kernel");
		eliminar_paquete(paquete);
		return;
	}

	log_info(logger, "Conexión exitosa");

	enviar_paquete(paquete, socket_cliente);
	log_info(logger, "Paquete enviado");

	eliminar_paquete(paquete);
	liberar_conexion(socket_cliente);
}
