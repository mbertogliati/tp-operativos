#include  "../include/operaciones.h"

//int escribir_en_memoria(uint8_t dato, int direccion){
//    if(direccion > (configuracion -> tam_memoria) - 1)
//        return -1;
//
//    memcpy(memoria_principal+direccion, &dato, sizeof(uint8_t));
//    return 1;
//}
//uint8_t* leer_de_memoria(int direccion){
//    uint8_t *posicion = (uint8_t *) (memoria_principal + direccion);
//    return posicion;
//}

//int crear_nueva_tabla(t_list* lista_de_tablas, int tamanio_del_proceso){
//
//}

void suspender_proceso(int id_proceso){
	//Dice escribir solamente la informacion necesaria, usar t_tabla2->M sin importar el algoritmo
	//de reemplazo?
	t_tabla2* tabla2;
	int marcos_counter = 0;
	for(int i = 0; i < (configuracion->tam_memoria / configuracion->tam_pagina); i++){
		tabla2 = list_get(tabla_planificacion, i);
		if(marcos_counter < configuracion->marcos_por_proceso && tabla2 != NULL && tabla2->id == id_proceso){
			tabla2->P = 0;
			escribir_pagina_SWAP(id_proceso, tabla2->pagina, i);
			marcos_counter++;
		}

	}
}

//Version con direccion de tabla1
void suspender_proceso2(int direccion){
	t_list* tabla1 = direccion;
	int marco_inicial = list_size(tabla_planificacion);

	for(int i = 0; i < list_size(tabla1); i++){
		t_list* tabla2 = list_get(tabla1, i);
		for(int j = 0; j < list_size(tabla2); j++){
			t_tabla2* pagina = list_get(tabla2, j);
			if(pagina->P){
				pagina->P = false;

				//Busco el primer marco del grupo asignado
				if(pagina->marco < marco_inicial)
					marco_inicial = pagina->marco;

				if(strcmp(configuracion->algoritmo_reemplazo, "CLOCK")){
					escribir_pagina_SWAP(pagina->id, pagina->pagina, pagina->marco);
				}else{
					if(pagina->M){
						escribir_pagina_SWAP(pagina->id, pagina->pagina, pagina->marco);
					}
				}
			}
		}
		//Actualizo todos los marcos a la vez para evitar errores con los semaforos
		if(marco_inicial != list_size(tabla_planificacion)){
			sem_wait(&mutex_tabla_planificacion);
			for(int i=0; i < configuracion->marcos_por_proceso; i++)
				list_replace(tabla_planificacion, marco_inicial + i, NULL);
			sem_post(&mutex_tabla_planificacion);
		}
	}
}

void finalizar_proceso(int direccion){
	t_list* tabla1 = direccion;
	int tamanio_tabla1 = list_size(tabla1);
	int marco_inicial = list_size(tabla_planificacion);
	int id_proceso;

	for(int i = 0; i < tamanio_tabla1; i++){
		t_list* tabla2 = list_get(tabla1, 0);
		int tamanio_tabla2 = list_size(tabla2);
		for(int j = 0; j < tamanio_tabla2; j++){
			t_tabla2* pagina = list_get(tabla2, 0);
			if(!pagina->P && pagina->marco < marco_inicial){
				marco_inicial = pagina->marco;
			}
			id_proceso = pagina->id;
			free(pagina);
			list_remove(tabla2, 0);
		}
		list_destroy(tabla2);
		list_remove(tabla1, 0);
	}

	//Actualizo todos los marcos a la vez para evitar problemas con los semaforos
	if(marco_inicial != list_size(tabla_planificacion)){
		sem_wait(&mutex_tabla_planificacion);
		for(int i = 0; i < configuracion->marcos_por_proceso; i++)
			list_replace(tabla_planificacion, marco_inicial + i, NULL);
		sem_post(&mutex_tabla_planificacion);
	}

	list_destroy(tabla1);

	char* SWAP_path = string_from_format("%s/%d.swap",
				configuracion->path_swap,
				id_proceso);
	remove(SWAP_path);

}

int obtener_tabla2(int direccion, int indice){
	t_list* tabla1 = direccion;

	return list_get(tabla1, indice);
}

int obtener_marco(int direccion, int indice){
	t_list* tabla2 = (t_list*)direccion;
	t_tabla2* pagina = list_get(tabla2, indice);

	//De no estar cargada en memoria la carga, devuelve el marco al final
	if(!pagina->P){
		int marco = ejecutar_algoritmo_de_reemplazo(pagina->pagina, pagina->id);

		//En caso de error se retorna inmediatamente -1
		if(marco == -1)
			return marco;

		pagina->marco = marco;
		pagina->P = 1;
		sem_wait(&mutex_tabla_planificacion);
		list_replace(tabla_planificacion, marco, pagina);
		sem_post(&mutex_tabla_planificacion);
	}
	//De ser referenciada se pone el bit de uso en 1
	pagina->U = 1;
	return pagina->marco;
}

//Como la lectura y escritura de memoria siempre va a ser despues de la funcion obtener_marco(),
//y no van a ocurrir reemplazo de paginas antes, se garantiza que la pagina siempre esta en memoria
void escribir_a_memoria(int direccion, int tamanio_a_escribir, void* a_escribir){
	char* puntero = memoria_principal;
	puntero += direccion;

	//Se pone el bit de modificacion(M) en 1
	t_tabla2* pagina = list_get(tabla_planificacion, direccion / configuracion->tam_pagina);
	pagina->M = true;
	memcpy(puntero, a_escribir, tamanio_a_escribir);
}

void* leer_de_memoria(int direccion, int tamanio_a_leer){

	void* puntero = memoria_principal;
	puntero += direccion;
	void* lectura = malloc(tamanio_a_leer);

	memcpy(lectura,puntero,tamanio_a_leer);

	return lectura;
}

void leer_pagina_SWAP(int id_proceso, int pagina, int marco){
	void* pagina_leida = malloc(configuracion->tam_pagina);
	FILE* SWAP_file = encontrar_SWAP(id_proceso);

	fseek(SWAP_file, pagina * configuracion->tam_pagina, SEEK_SET);
	fread(pagina_leida, configuracion->tam_pagina, 1, SWAP_file);

	//El tamaño de char es un byte, por lo que esto mueve tantos bytes como tamaño de pagina * paginas hasta el marco
	char* puntero = memoria_principal;
	puntero += marco * configuracion->tam_pagina;

	memcpy(puntero, pagina_leida, configuracion->tam_pagina);

	free(pagina_leida);
	fclose(SWAP_file);

	//Espera por acceso a disco
	sleep(0.001 * configuracion->retardo_swap);

}

void escribir_pagina_SWAP(int id_proceso, int pagina, int marco){
	FILE* SWAP_file = encontrar_SWAP(id_proceso);

	char* puntero = memoria_principal;
	puntero += marco * configuracion->tam_pagina;

	fseek(SWAP_file, pagina * configuracion->tam_pagina, SEEK_SET);
	fwrite(puntero, configuracion->tam_pagina, 1, SWAP_file);

	fclose(SWAP_file);

	//Espera por acceso a disco
	sleep(0.001 * configuracion->retardo_swap);
}
