#ifndef CONSOLA_INCLUDE_CONSOLA_H_
#define CONSOLA_INCLUDE_CONSOLA_H_

#include "lista_de_instrucciones.h"
#include "../../shared/cliente/utils.h"


void consola(char *path, int tamanio);
t_list *leer_archivo(char *path);
t_paquete *crear_paquete_instrucciones(t_link_element *lista_de_instrucciones, int tamanio);
void enviar_paquete_instrucciones(t_paquete *paquete);
int conectar_a_kernel();
bool son_argumentos_validos(int cantidad_argumentos, char** argv);

#endif /* CONSOLA_INCLUDE_CONSOLA_H_ */
