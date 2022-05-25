#ifndef LEER_ARCHIVO_H_
#define LEER_ARCHIVO_H_

#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include "../../shared/include/estructuras/instrucciones.h"
#include "../../shared/include/sockets/client_utils.h"

extern t_log *logger;

t_paquete *crear_paquete_instrucciones(char *path, uint32_t tamanio);

#endif
