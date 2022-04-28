#ifndef LEER_ARCHIVO_H_
#define LEER_ARCHIVO_H_

#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include "../../shared/include/instrucciones.h"

extern t_log *logger;

t_list *leer_archivo(char *path);

#endif
