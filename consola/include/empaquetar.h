#ifndef EMPAQUETAR_H_
#define EMPAQUETAR_H_

#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include "../../shared/include/instrucciones.h"
#include "../../shared/include/client_utils.h"

extern t_log *logger;

t_paquete *crear_paquete_instrucciones(t_list *instrucciones, int tamanio);
void enviar_paquete_instrucciones(t_paquete *paquete);

#endif
