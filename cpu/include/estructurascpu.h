#ifndef ESTRUCTURASCPU_H_
#define ESTRUCTURASCPU_H_

typedef struct{
    int entradas_tlb;
    char* reemplazo_tlb;
    int retardo_noop;
    char* ip_memoria;
    char* puerto_memoria;
    char* puerto_escucha_dispatch;
    char* puerto_escucha_interrupt;
}t_config_cpu;


#endif