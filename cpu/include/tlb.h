#ifndef TLB_H_
#define TLB_H_
#include "estructurascpu.h"
#include <commons/collections/list.h>
#include <stdlib.h>
typedef struct {
    int pagina;
    int marco;
} t_tlb;

t_list* TLB;
int chequear_tlb(int pagina);
void actualizar_tlb(int pagina, int marco);

#endif
