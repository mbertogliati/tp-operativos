#ifndef LEER_ARCHIVO_H_
#define LEER_ARCHIVO_H_
#include <stdlib.h>
#include <commons/string.h>
#include <stdbool.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include "../../shared/include/instrucciones.h"
#include "../../shared/include/client_utils.h"
t_log *logger;

t_list *leer_archivo(char *path);

#endif