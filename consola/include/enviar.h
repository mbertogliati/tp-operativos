#ifndef EMPAQUETAR_H_
#define EMPAQUETAR_H_

#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <sockets/client_utils.h>

extern t_log *logger;

void enviar_paquete_instrucciones(t_paquete *paquete);

#endif
