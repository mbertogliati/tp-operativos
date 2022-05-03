#include "../include/estructuras.h"
#include "../include/paginacion.h"

t_config_memoria *cargar_configuraciones(char** path){
    t_config *config_memoria = NULL;
    t_config_memoria *configuracion;

    memoria_log = log_create("logs/memoria.log","MEMORIA",true,LOG_LEVEL_INFO);

    if(!(config_memoria = config_create("config/memoria.config"))){
        log_error(memoria_log,"ERROR - No se encontro el archivo de configuracion de memoria");
        return NULL;
    }

    if(!es_configuracion_valida(config_memoria)) 
        return NULL;

    configuracion->tam_memoria = config_get_int_value(config_memoria,"TAM_MEMORIA");
    configuracion->tam_pagina = config_get_int_value(config_memoria,"TAM_PAGINA");
    configuracion->entradas_por_tabla = config_get_int_value(config_memoria,"ENTRADAS_POR_TABLA");
    configuracion->retardo_memoria = config_get_int_value(config_memoria,"RETARDO_MEMORIA");
    configuracion->retardo_swap = config_get_int_value(config_memoria,"RETARDO_SWAP");
    configuracion->marcos_por_proceso = config_get_int_value(config_memoria,"MARCOS_POR_PROCESO");

    configuracion->puerto = string_new();
    string_append(&configuracion->puerto, config_get_string_value(config_memoria,"PUERTO_ESCUCHA"));

    configuracion->algoritmo_reemplazo = string_new();
    string_append(&configuracion->algoritmo_reemplazo, config_get_string_value(config_memoria,"ALGORITMO_REEMPLAZO"));

    configuracion->path_swap = string_new();
    string_append(&configuracion->path_swap, config_get_string_value(config_memoria,"PATH_SWAP"));
    
    return config_memoria;
}

bool es_configuracion_valida(t_config *config_memoria){
    char claves_validas[9][20+1] = {"TAM_MEMORIA","TAM_PAGINA","ENTRADAS_POR_TABLA",
                                "RETARDO_MEMORIA","RETARDO_SWAP","MARCOS_POR_PROCESO",
                                "PUERTO_ESCUCHA","ALGORITMO_REEMPLAZO","PATH_SWAP"};
    for(int i = 0; i < 0; i++){
        if (!config_has_property(config_memoria, claves_validas[i])){
            log_error(memoria_log,"ERROR - Archivo de configuracion invalido");
            return false;
        }
    }
    if (config_keys_amount(config_memoria) >9 ){
        log_error(memoria_log,"ERROR - Archivo de configuracion invalido");
        return false;
    }
    return true;
}

void crear_SWAP(){

}

t_tabla2* inicializar_tabla2(){
	t_tabla2* tabla = malloc(sizeof(t_tabla2));
	tabla->M = 0;
	tabla->P = 0;
	tabla->U = 0;
	return tabla;
}

void crear_metadata(int id_proceso, int tamanio_proceso){
	proceso* nuevo = malloc(sizeof(proceso));
	nuevo->id = id_proceso;
	nuevo->tablas1 = list_create();


	int cant_paginas = ceil(tamanio_proceso / configuracion_memoria->tam_pagina);
	int cant_tablas2 = ceil(cant_paginas / configuracion_memoria->entradas_por_tabla);
	int cant_tablas1 = ceil(cant_tablas2 / configuracion_memoria->entradas_por_tabla);
	int page_counter = 0;
	int tabla2_counter = 0;
	for(int i = 0; i < cant_tablas1; i++){
		t_list* tabla1 = list_create();
		for(int j = 0; j < configuracion_memoria->entradas_por_tabla; j++){
			if(tabla2_counter < cant_tablas2){
				t_list* tabla2 = list_create();
				for(int w = 0; w < configuracion_memoria->entradas_por_tabla; w++){
					if(page_counter < cant_paginas){
					list_add(tabla2, inicializar_tabla2());
					page_counter++;
					}
				}
				list_add(tabla1, tabla2);
				tabla2_counter++;
			}
		}
		list_add(nuevo->tablas1, tabla1);
	}
	list_add(procesos, nuevo);
}

void agregar_proceso(int id_proceso, int tamanio_proceso){
	crear_SWAP();
	crear_metadata(id_proceso, tamanio_proceso);
}

t_tabla2* encontrar_tabla2(int proceso_id, int entrada_primer_nivel, int entrada_segundo_nivel){

//	bool mismo_proceso(proceso proceso_a_comparar){
//		return proceso_a_comparar.id == proceso_id;
//	}
//
//	t_list* proceso = list_find(procesos, mismo_proceso());
//	t_list* tabla1 = list_get(proceso, entrada_primer_nivel);
//	t_list* tabla2 = list_get(tabla1, entrada_segundo_nivel);
//
//	return tabla2;
	t_tabla2* respuesta;
	return respuesta;
}



//Funciones viejas

