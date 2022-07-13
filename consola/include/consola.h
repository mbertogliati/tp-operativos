#ifndef CONSOLA_H_
#define CONSOLA_H_

#include "conexion_kernel.h"
#include "crear_paquete.h"

t_log *logger;

FILE *son_argumentos_validos(int cantidad_argumentos, char **argv);

#endif /* CONSOLA_H_ */ 
