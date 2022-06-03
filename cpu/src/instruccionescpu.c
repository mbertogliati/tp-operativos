#include "../include/instruccionescpu.h"


void ciclo_de_instruc(){

    while(true){

    }

}
t_instruccion fetch(t_pcb* pcb){
    t_instruccion instruccion_nueva;
    instruccion_nueva = *( (t_instruccion*) list_get(&(pcb->instrucciones), pcb->program_counter));
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
    int resto = (*tam_de_pagina) % desplazamiento;
    int direccion_fisica = obtener_direccion_fisica(socket_memoria, tabla_paginas, direccion_logica, *entradas_por_tabla, *tam_de_pagina);;
    void* valor_leido;
    uint32_t retorno;
    if(resto == 0){
        valor_leido = pedir_lectura(socket_memoria, 4, direccion_fisica);
        retorno = *(uint32_t *) valor_leido;
        free(valor_leido);
        return retorno;
    }





}