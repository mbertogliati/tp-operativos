#ifndef EMPAQUETAR_H_
#define EMPAQUETAR_H_

#include "../include/leer_archivo.h"
#include <commons/config.h>

t_paquete *crear_paquete_instrucciones(t_list *instrucciones, int tamanio);
void enviar_paquete_instrucciones(t_paquete *paquete);

#endif