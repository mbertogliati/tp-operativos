#include "../include/leer_config.h"

void crear_config() {
	kernel_config = config_create("kernel.config");
}

void destruir_config() {
	config_destroy(kernel_config);
}

// memoria
char *ip_memoria() {
	return config_get_string_value(kernel_config, "IP_MEMORIA");
}
char *puerto_memoria() {
	return config_get_string_value(kernel_config, "PUERTO_MEMORIA");
}

// cpu
char *ip_cpu() {
	return config_get_string_value(kernel_config, "IP_CPU");
}
char *puerto_cpu_dispatch() {
	return config_get_string_value(kernel_config, "PUERTO_CPU_DISPATCH");
}
char *puerto_cpu_interrupt() {
	return config_get_string_value(kernel_config, "PUERTO_CPU_INTERRUPT");
}

// kernel
char *puerto_escucha() {
	return config_get_string_value(kernel_config, "PUERTO_ESCUCHA");
}

// planificacion
char *algoritmo_planificacion() {
	return config_get_string_value(kernel_config, "ALGORITMO_PLANIFICACION");
}
int grado_multiprogramacion() {
	return config_get_int_value(kernel_config, "GRADO_MULTIPROGRAMACION");
}
double tiempo_max_bloqueado() {
	return config_get_double_value(kernel_config, "TIEMPO_MAXIMO_BLOQUEADO");
}

// sjf
double estimacion_inicial() {
	return config_get_double_value(kernel_config, "ESTIMACION_INICIAL");
}
double alfa() {
	return config_get_double_value(kernel_config, "ALFA");
}
