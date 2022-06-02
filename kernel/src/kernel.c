#include "../include/kernel.h"

int main() {
	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);
	config = config_create("kernel.config");

	inicializar_colas();

	conectar_memoria(ip_memoria(), puerto_memoria());
	conectar_consola(puerto_escucha());
	conectar_cpu(ip_cpu(), puerto_cpu_dispatch(), puerto_cpu_interrupt);

	desconectar_memoria();

	log_destroy(logger);
	config_destroy(config);

	return EXIT_SUCCESS;
}
