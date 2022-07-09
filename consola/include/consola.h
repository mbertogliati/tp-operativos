#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdbool.h>

#include "conexion_kernel.h"
#include "crear_paquete.h"

t_log *logger;

bool son_argumentos_validos(int cantidad_argumentos, char **argv);

#endif /* CONSOLA_H_ */ 
