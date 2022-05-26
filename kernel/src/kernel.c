#include "../include/kernel.h"

int main() {
	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);
	crear_config();
	conectar_consola();
	log_destroy(logger);
	destruir_config();
	return EXIT_SUCCESS;
}
