#ifndef OPERACIONES_H_
#define OPERACIONES_H_
#include "estructuras.h"
#include <stdint.h>
#include <string.h>
void suspender_proceso(int id_proceso);
void suspender_proceso2(int direccion);
void finalizar_proceso(int direccion);
int obtener_tabla2(int direccion, int indice);
int obtener_marco(int direccion, int indice);
void escribir_a_memoria(int direccion, int tamanio_a_escribir, void* a_escribir);
void* leer_de_memoria(int direccion, int tamanio_a_leer);

#endif