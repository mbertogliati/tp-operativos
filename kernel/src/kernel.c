#include "../include/kernel.h"

struct {
	char *ip_memoria;
	char *puerto_memoria;
	char *ip_cpu;
	struct {
		char *dispatch;
		char *interrupt;
	} puerto_cpu;
	char *puerto_escucha;
	char *algoritmo_planificacion;
	double estimacion_inicial;
	double alfa;
	int grado_multiprogramacion;
	double tiempo_max_bloqueado;
} configs;

void leer_configs() {
	t_config *kernel_config = config_create("kernel/kernel.config");
	configs.ip_memoria = config_get_string_value(kernel_config, "IP_MEMORIA");
	configs.puerto_memoria = config_get_string_value(kernel_config, "PUERTO_MEMORIA");
	configs.ip_cpu = config_get_string_value(kernel_config, "IP_CPU");
	configs.puerto_cpu.dispatch = config_get_string_value(kernel_config, "PUERTO_CPU_DISPATCH");
	configs.puerto_cpu.interrupt = config_get_string_value(kernel_config, "PUERTO_CPU_INTERRUPT");
	configs.puerto_escucha = config_get_string_value(kernel_config, "PUERTO_ESCUCHA");
	configs.algoritmo_planificacion = config_get_string_value(kernel_config, "ALGORITMO_PLANIFICACION");
	configs.estimacion_inicial = config_get_double_value(kernel_config, "ESTIMACION_INICIAL");
	configs.alfa = config_get_double_value(kernel_config, "ALFA");
	configs.grado_multiprogramacion = config_get_int_value(kernel_config, "GRADO_MULTIPROGRAMACION");
	configs.tiempo_max_bloqueado = config_get_double_value(kernel_config, "TIEMPO_MAXIMO_BLOQUEADO");
}

int main() {
	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);
	leer_configs();

	int socket_servidor = iniciar_servidor(configs.puerto_escucha);
	log_info(logger, "Servidor listo para recibir al cliente");
	int socket_cliente = esperar_cliente(socket_servidor);

	t_pcb *pcb;
	char *mensaje;

	while (1) {
		switch (recibir_operacion(socket_cliente)) {
		case INSTRUCCIONES_CONSOLA:
			log_info(logger, "Se conectó un cliente!");
			bloque_control = generar_pcb(socket_cliente);
			log_info(logger, "Recibí el proceso:\n");
			imprimir_pcb(bloque_control);
			list_destroy_and_destroy_elements(bloque_control->instrucciones, (void *) liberar_instruccion);
			break;

		case MENSAJE:
			mensaje = recibir_mensaje(socket_cliente);
			log_info(logger, "Me llegó el mensaje: %s", mensaje);
			free(mensaje);
			break;

		case -1:
			log_error(logger, "El cliente se desconectó. Terminando servidor");
			return EXIT_FAILURE;

		default:
			log_warning(logger,"Operación desconocida");
			break;
		}
	}

	log_destroy(logger);
	return EXIT_SUCCESS;
}
