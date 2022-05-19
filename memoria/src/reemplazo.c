#include "../include/reemplazo.h"

int clock_normal(int id_proceso, int pagina){

    int size_tabla_planificacion = list_size(tabla_planificacion);
    t_tabla2 *pagina_actual;

    //Si en un ciclo previo se lo aumento a mas de la cantidad de marcos, se lo devuelve a 0
    if(planificacion_ptr == size_tabla_planificacion)
    	planificacion_ptr = 0;

    for(int j=0; j<2; j++){
        for(;planificacion_ptr < size_tabla_planificacion ;planificacion_ptr++){//Recorre toda la lista de marcos creada por vos

            pagina_actual = list_get(tabla_planificacion, planificacion_ptr);
            if (!pagina_actual){ //Si el marco actual esta vacio se lo llena
            	leer_pagina_SWAP(id_proceso, pagina, planificacion_ptr);
            	planificacion_ptr++;
            	return planificacion_ptr - 1;
            }
            
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
                //Se aumenta el planificacion_ptr para que apunte al proximo marco
                planificacion_ptr++;
                //Devuelvo el marco para que los que me llamaron sepan cual de todos elegi reemplazar:
                return pagina_actual -> marco;
            }
        }
        planificacion_ptr = 0;
    }//El for de "j" asegura que si no se puede elegir un marco para reemplazar a la primera vuelta, se haga una segunda.
    //Si a las 2da vuelta no se reemplaza nada, se rompe todo
    return -1;

}

int clock_modificado(int id_proceso, int pagina){
    int size_tabla_planificacion = list_size(tabla_planificacion);
    t_tabla2 *pagina_actual;

    if(planificacion_ptr == size_tabla_planificacion)
    	planificacion_ptr = 0;

    for (int j=0; j<2; j++){
        for(int i = 0; i < size_tabla_planificacion; i++){//Recorre toda la lista de marcos creada por vos

            pagina_actual = list_get(tabla_planificacion, planificacion_ptr);
            if (!pagina_actual){//Si el marco esta vacio se lo llena
            	leer_pagina_SWAP(id_proceso, pagina, planificacion_ptr);
            	planificacion_ptr++;
            	return planificacion_ptr - 1;

            }
            
            if(!pagina_actual->U && !pagina_actual->M){//Si los dos son 0 da falso y se reemplaza
                leer_pagina_SWAP(id_proceso, pagina, pagina_actual -> marco);//En este caso no escribimos porque M=0
                pagina_actual -> P = false;
                planificacion_ptr++;
                return pagina_actual -> marco;
            }

            if(planificacion_ptr == size_tabla_planificacion - 1)
            	planificacion_ptr = 0;
            else
            	planificacion_ptr++;
        }//Si llega aca, estamos en el paso (2)
    
        for(int i=0; i < size_tabla_planificacion ;i++){//Recorre toda la lista de marcos creada por vos

            pagina_actual = list_get(tabla_planificacion, planificacion_ptr);
            if (!pagina_actual){//Si es null se salta la iteracion
            	if(planificacion_ptr == size_tabla_planificacion - 1)
            		planificacion_ptr = 0;
            	else
            	    planificacion_ptr++;
            	continue;
            }
            
            if(pagina_actual->M && !pagina_actual->U){//Solo se cumple si M=1 y U=0
                escribir_pagina_SWAP(id_proceso, pagina, pagina_actual -> marco);//Ahora si escribimos
                leer_pagina_SWAP(id_proceso, pagina, pagina_actual -> marco);
                pagina_actual -> P = false;
                planificacion_ptr++;
                return pagina_actual -> marco;
            }
            else
                pagina_actual -> U = false; //En este paso si se cambian los bits U
            if(planificacion_ptr == size_tabla_planificacion - 1)
            	planificacion_ptr = 0;
            else
            	planificacion_ptr++;
        }
    }//Si llega aca tiene que hacer otra vuelta, sino se rompe todo como antes
    return -1;
}

int ejecutar_algoritmo_de_reemplazo(int pagina, int id_proceso){

    if (strcmp(configuracion -> algoritmo_reemplazo, "CLOCK") == 0)
        return clock_normal (id_proceso, pagina);

    return clock_modificado (id_proceso, pagina);

}

