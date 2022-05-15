#ifndef ESTRUCTURASCPU_H_
#define ESTRUCTURASCPU_H_
#include <commons/config.h>
#include <commons/log.h>
#include <stdbool.h>
#include <string.h>


typedef struct{
    int entradas_tlb;
    char* reemplazo_tlb;
    int retardo_noop;
    char* ip_memoria;
    char* puerto_memoria;
    char* puerto_escucha_dispatch;
    char* puerto_escucha_interrupt;
}t_config_cpu;

t_log *cpu_log;
t_config_cpu* cpuconfig;

bool es_configuracion_valida(t_config *config_cpu);
t_config_cpu *cargar_config (char** path);

#endif