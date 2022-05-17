#include "../include/reemplazo.h"

int clock_normal(int id_proceso, int pagina){

    int size_tabla_planificacion = list_size(tabla_planificacion);
    t_tabla2 *pagina_actual;

    for(int j=0; j<2; j++){
        for(int i=0; i < size_tabla_planificacion ;i++){//Recorre toda la lista de marcos creada por vos

            pagina_actual = list_get(tabla_planificacion, i);
            if (!pagina_actual) //Si es null se salta la iteracion
                continue;
            
            if (pagina_actual->U)
                pagina_actual->U = false; //Si U=1 -> U=0
            else{
                //Se escribe la pagina en swap por si hubo modificacion:
                escribir_pagina_SWAP(pagina_actual->id, pagina_actual->pagina, pagina_actual->marco);
                //Se lee la pagina de swap y se la pone en el marco (en memoria): 
                leer_pagina_SWAP(id_proceso, pagina, pagina_actual -> marco);
                //Ahora se marca esa pagina como ausente, se deja la actualizacion de las tablas con la nueva pagina cargada a cargo
                //de quien me haya llamado y tenga los datos necesarios, yo no los tengo
                pagina_actual -> P = false;
                //Devuelvo el marco para que los que me llamaron sepan cual de todos elegi reemplazar:
                return pagina_actual -> marco;
            }
        }
    }//El for de "j" asegura que si no se puede elegir un marco para reemplazar a la primera vuelta, se haga una segunda.
    //Si a las 2da vuelta no se reemplaza nada, se rompe todo
    return -1;

}
int clock_modificado(int id_proceso, int pagina){
    int size_tabla_planificacion = list_size(tabla_planificacion);
    t_tabla2 *pagina_actual;

    for (int j=0; j<2; j++){
        for(int i=0; i < size_tabla_planificacion ;i++){//Recorre toda la lista de marcos creada por vos

            pagina_actual = list_get(tabla_planificacion, i);
            if (!pagina_actual) //Si es null se salta la iteracion
                continue;
            
            if(pagina_actual->U || pagina_actual->M)//Si los dos son 0 da falso y se reemplaza
                continue;
            else{
                leer_pagina_SWAP(id_proceso, pagina, pagina_actual -> marco);//En este caso no escribimos porque M=0
                pagina_actual -> P = false;
                return pagina_actual -> marco;
            }
        }//Si llega aca, estamos en el paso (2)

        for(int i=0; i < size_tabla_planificacion ;i++){//Recorre toda la lista de marcos creada por vos

            pagina_actual = list_get(tabla_planificacion, i);
            if (!pagina_actual) //Si es null se salta la iteracion
                continue;
            
            if(pagina_actual->M && !pagina_actual->U){//Solo se cumple si M=1 y U=0
                escribir_pagina_SWAP(id_proceso, pagina, pagina_actual -> marco);//Ahora si escribimos
                leer_pagina_SWAP(id_proceso, pagina, pagina_actual -> marco);
                pagina_actual -> P = false;
                return pagina_actual -> marco;
            }
            else
                pagina_actual -> U = false; //En este paso si se cambian los bits U
        }
    }//Si llega aca tiene que hacer otra vuelta, sino se rompe todo como antes
    return -1;
}
int ejecutar_algoritmo_de_reemplazo(int pagina, int id_proceso){

    if (strcmp(configuracion -> algoritmo_reemplazo, "CLOCK") == 0)
        return clock_normal (id_proceso, pagina);

    return clock_modificado (id_proceso, pagina);

}

