#include "../include/tlb.h"

int chequear_tlb(int pagina){
    t_tlb* entrada_tlb;

    entrada_tlb = NULL;
    int indice = 0;
    t_list_iterator* iterador_tlb = list_iterator_create(TLB);

    while(list_iterator_has_next(iterador_tlb)){
        entrada_tlb = list_iterator_next(iterador_tlb);
        indice = iterador_tlb->index;
        if(entrada_tlb->pagina == pagina)
            break;
    }

    if(entrada_tlb ==  NULL)
        return -1;
    
    if(strcmp((cpuconfig->reemplazo_tlb),"LRU")==0){
        list_remove(TLB, indice);
        list_add(TLB,entrada_tlb);
    }
    
    list_iterator_destroy(iterador_tlb);
    return entrada_tlb;
}

void actualizar_tlb(int pagina, int marco){

    t_tlb* nueva_entrada = malloc(sizeof(t_tlb));
    nueva_entrada->pagina = pagina;
    nueva_entrada->marco = marco;

    if(list_size(TLB) < cpuconfig->entradas_tlb){
        list_add(TLB, nueva_entrada);
        return;
    }
    list_remove_and_destroy_element(TLB, 0,free);
    list_add(TLB, nueva_entrada);
    return;
}
