#include "../include/leer_config.h"

void leer_configs() {
	config = config_create("kernel.config");
	configs.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	configs.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	configs.ip_cpu = config_get_string_value(config, "IP_CPU");
	configs.puerto_cpu_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
	configs.puerto_cpu_interrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
	configs.puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
	configs.algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	configs.grado_multiprogramacion = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
	configs.tiempo_max_bloqueado = config_get_double_value(config, "TIEMPO_MAXIMO_BLOQUEADO");
	configs.estimacion_inicial = config_get_double_value(config, "ESTIMACION_INICIAL");
	configs.alfa = config_get_double_value(config, "ALFA");
}
