#ifndef PAGINACION_H_
#define PAGINACION_H_
#include <commons/log.h>
#include <commons/collections/list.h>


typedef struct{
	int id;
	t_list* tablas1;
} proceso;


typedef struct{
    int marco;
    bool P;
    bool U;
    bool M;
} t_tabla2;



#endif
