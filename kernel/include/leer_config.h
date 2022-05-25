#ifndef LEER_CONFIG_H_
#define LEER_CONFIG_H_

#include <commons/config.h>

extern t_config *kernel_config;

char *ip_memoria();
char *puerto_memoria();
char *ip_cpu();
char *puerto_cpu_dispatch();
char *puerto_cpu_interrupt();
char *puerto_escucha();
char *algoritmo_planificacion();
double estimacion_inicial();
double alfa();
int grado_multiprogramacion();
double tiempo_max_bloqueado();

#endif /* LEER_CONFIG_H_ */
