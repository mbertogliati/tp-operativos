#ifndef LEER_ARCHIVO_H_
#define LEER_ARCHIVO_H_

#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "../../shared/include/instrucciones.h"
#include "../../shared/include/client_utils.h"

extern t_log *logger;

t_paquete *crear_paquete_instrucciones(char *path, int tamanio);

#endif
