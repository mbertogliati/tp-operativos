#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdbool.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include "../../shared/include/instrucciones.h"
#include "../../shared/include/client_utils.h"

t_log *logger;

bool son_argumentos_validos(int cantidad_argumentos, char **argv);
void consola(char *path, int tamanio);

t_list *leer_archivo(char *path);
t_paquete *crear_paquete_instrucciones(t_list *instrucciones);
void enviar_paquete_instrucciones(t_paquete *paquete, int tamanio);

#endif /* CONSOLA_H_ */
