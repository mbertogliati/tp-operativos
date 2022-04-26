#ifndef LISTA_DE_INSTRUCCIONES_H_
#define LISTA_DE_INSTRUCCIONES_H_

#include <commons/collections/list.h>

typedef unsigned int uint32_t;

typedef enum {
	NO_OP, IO, READ, WRITE, COPY, EXIT
} id;

typedef struct {
	id identificador;
	int cant_parametros;
	uint32_t *parametros;
} t_instruccion;

void procesar_linea(char **instruccion_leida, t_list *lista_de_instrucciones);
t_instruccion *crear_instruccion(id identificador, int cant_parametros, uint32_t *parametros);
id get_identificador(char* identificador_leido);

void leer_lista(t_list *lista_de_instrucciones);
void leer_instruccion(t_instruccion *instruccion);

void liberar_lista(t_list *lista_de_instrucciones);
void liberar_instruccion(t_instruccion *instruccion);

#endif /* LISTA_DE_INSTRUCCIONES_H_ */
