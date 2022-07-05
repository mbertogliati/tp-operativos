#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdbool.h>

#include "crear_paquete.h"
#include "enviar.h"

t_log *logger;

bool son_argumentos_validos(int cantidad_argumentos, char **argv);
bool terminar();

#endif /* CONSOLA_H_ */ 
