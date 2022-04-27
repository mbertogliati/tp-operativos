#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

#include <commons/collections/list.h>

typedef unsigned int uint32_t;

typedef enum {
	NO_OP, IO, READ, WRITE, COPY, EXIT
} id;

typedef struct {
	int identificador;
	int cant_parametros;
	uint32_t *parametros;
} t_instruccion;

t_instruccion *crear_instruccion(int identificador, int cant_parametros, uint32_t *parametros);
int get_identificador(char* identificador_leido);

void leer_instruccion(t_instruccion *instruccion);
void leer_lista(t_list *lista_de_instrucciones);

void liberar_instruccion(t_instruccion *instruccion);
void liberar_lista(t_list *lista_de_instrucciones);

#endif /* INSTRUCCIONES_H_ */
