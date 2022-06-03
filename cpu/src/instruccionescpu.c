#include "../include/instruccionescpu.h"

bool proceso_terminado;

void ciclo_de_instruccion(t_pcb* pcb){

    t_instruccion instruccion_actual;
    uint32_t hayIO;
    t_buffer* buffer = malloc(sizeof(t_buffer));

    while(pcb){
        instruccion_actual = fetch(pcb);
        if(decode(instruccion_actual));
            //fetch_operand();
        hayIO = execute(instruccion_actual);
        if(proceso_terminado){
            enviar_pcb(pcb, socket_dispatch, 0);
            liberar_pcb(pcb);
            continue;
        }
        if(hayIO){
            enviar_pcb(pcb, socket_dispatch, hayIO);
            continue;
        }
        sem_wait(&mutex_interrupt);
        if(check_interrupt){
            check_interrupt = false;
            sem_post(&mutex_interrupt);

            liberar_pcb(pcb);
            buffer->stream = recibir_buffer(buffer->size, socket_dispatch);
            pcb = desempaquetar_pcb(buffer->stream);
        }        
    }
    free(buffer);


}
t_instruccion fetch(t_pcb* pcb){
    t_instruccion instruccion_nueva;
    instruccion_nueva = *( (t_instruccion*) list_get(&(pcb->instrucciones), pcb->program_counter));
    pcb->program_counter++;
    return instruccion_nueva;
}
bool decode(t_instruccion instruccion){
    if (instruccion.identificador == COPY) return true;
    else return false;
}
uint32_t fetch_operand(int socket_memoria, int tabla_paginas, uint32_t direccion_logica){//Todavia no esta implementado por TLB
    //Antes habria que chequear que la direccion fisica este en TLB
    int direccion_fisica = obtener_direccion_fisica(socket_memoria, tabla_paginas, direccion_logica, *entradas_por_tabla, *tam_de_pagina);
    
}
uint32_t leer_desde_memoria(int socket_memoria, int tabla_paginas, uint32_t direccion_logica){

    int nro_de_pagina = floor( ((double) direccion_logica) / ((double) *tam_de_pagina));
    int desplazamiento = direccion_logica - nro_de_pagina * (*tam_de_pagina);
    int direccion_fisica = obtener_direccion_fisica(socket_memoria, tabla_paginas, direccion_logica, *entradas_por_tabla, *tam_de_pagina);
    void* valor_leido;
    int diferencia = desplazamiento+4 - *tam_de_pagina;
    uint32_t retorno;
    if(diferencia <= 0)
        valor_leido = pedir_lectura(socket_memoria, 4, direccion_fisica);
    
    else{
        valor_leido = pedir_lectura(socket_memoria, 4-diferencia ,direccion_fisica);
        valor_leido = realloc(valor_leido, 4);
        nro_de_pagina += 1;
        int nueva_direccion_logica = nro_de_pagina * (*tam_de_pagina);
        direccion_fisica = obtener_direccion_fisica(socket_memoria, tabla_paginas, nueva_direccion_logica, *entradas_por_tabla, *tam_de_pagina);
        memcpy (valor_leido+4-diferencia, pedir_lectura(socket_memoria, diferencia, direccion_fisica), diferencia);
    }

    retorno = *(uint32_t *) valor_leido;
    free(valor_leido);
    return retorno;

}
uint32_t execute(t_instruccion instruccion){
    switch (instruccion.identificador)
    {
    case NO_OP:
        sleep(0.001 * cpuconfig->retardo_noop);
        break;
    case IO:
        return instruccion.parametros[0];
        break;
    case EXIT:
        proceso_terminado = true;
        break;
    default:
        break;
    }
    return 0;
}