#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

#include <stdint.h>

typedef enum {
	NO_OP, IO, READ, WRITE, COPY, EXIT, NO_RECONOCIDO
} id;

typedef struct {
	uint8_t identificador;
	uint8_t cant_parametros;
	uint32_t *parametros;
} t_instruccion;

uint8_t get_identificador(char *);
uint8_t get_cant_parametros(uint8_t);
t_instruccion *crear_instruccion(uint8_t, uint8_t, uint32_t *);
void imprimir_instruccion(t_instruccion *);
void liberar_instruccion(t_instruccion *);
t_instruccion *desempaquetar_instruccion(void *, int *);

#endif /* INSTRUCCIONES_H_ */
