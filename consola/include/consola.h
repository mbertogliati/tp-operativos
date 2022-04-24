#ifndef CONSOLA_H_
#define CONSOLA_H_

	#include <commons/collections/list.h>

	typedef unsigned int uint4_t;

	typedef enum {
		NO_OP, IO, READ, COPY, WRITE, EXIT
	} id;

	typedef struct {
		id identificador;
		int cant_parametros;
		uint4_t *parametros;
	} t_instruccion;

	void consola(char *path, int tamanio);
	void leer_archivo(char *path, t_list *lista_de_instrucciones);
	t_instruccion *crear_instruccion(char **instruccion_leida);
	id get_identificador(char* identificador_leido);
	int cantidad_de_parametros(id identificador);

	void leer_lista(t_list *lista_de_instrucciones);
	void leer_instruccion(void *instruccion);

	void liberar_lista(t_list *lista_de_instrucciones);
	void liberar_instruccion(void *instruccion);

#endif
