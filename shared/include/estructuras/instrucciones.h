#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

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
void imprimir_instruccion(t_instruccion *instruccion);
void liberar_instruccion(t_instruccion *instruccion);

#endif /* INSTRUCCIONES_H_ */
