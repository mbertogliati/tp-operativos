#include "../include/leer_config.h"

char *ip_memoria() {
	return config_get_string_value(kernel_config, "IP_MEMORIA");
}
char *puerto_memoria() {
	return config_get_string_value(kernel_config, "PUERTO_MEMORIA");
}
char *ip_cpu() {
	return config_get_string_value(kernel_config, "IP_CPU");
}
char *puerto_cpu_dispatch() {
	return config_get_string_value(kernel_config, "PUERTO_CPU_DISPATCH");
}
char *puerto_cpu_interrupt() {
	return config_get_string_value(kernel_config, "PUERTO_CPU_INTERRUPT");
}
char *puerto_escucha() {
	return config_get_string_value(kernel_config, "PUERTO_ESCUCHA");
}
char *algoritmo_planificacion() {
	return config_get_string_value(kernel_config, "ALGORITMO_PLANIFICACION");
}
double estimacion_inicial() {
	return config_get_double_value(kernel_config, "ESTIMACION_INICIAL");
}
double alfa() {
	return config_get_double_value(kernel_config, "ALFA");
}
int grado_multiprogramacion() {
	return config_get_int_value(kernel_config, "GRADO_MULTIPROGRAMACION");
}
double tiempo_max_bloqueado() {
	return config_get_double_value(kernel_config, "TIEMPO_MAXIMO_BLOQUEADO");
}
