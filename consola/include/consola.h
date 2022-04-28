#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdbool.h>
#include "../include/leer_archivo.h"
#include "../include/empaquetar.h"

t_log *logger;

bool son_argumentos_validos(int cantidad_argumentos, char **argv);
void consola(char *path, int tamanio);

#endif /* CONSOLA_H_ */ 
