#ifndef LEER_CONFIG_H_
#define LEER_CONFIG_H_

#include <commons/config.h>

struct kernel_configs {
	char *ip_memoria;
	char *puerto_memoria;
	char *ip_cpu;
	char *puerto_cpu_dispatch;
	char *puerto_cpu_interrupt;
	char *puerto_escucha;
	char *algoritmo_planificacion;
	int grado_multiprogramacion;
	double tiempo_max_bloqueado;
	double estimacion_inicial;
	double alfa;
} configs;

extern t_config *config;

#endif /* LEER_CONFIG_H_ */
