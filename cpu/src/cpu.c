#include "../include/cpu.h"

int main(void){

    t_config *iconfig;
    iconfig = config_create("config/cpu.config");

    t_config_cpu *cpuconfig;
    cpuconfig -> entradas_tlb = config_get_int_value(iconfig, "ENTRADAS_TLB");
    cpuconfig -> reemplazo_tlb = config_get_string_value(iconfig, "REEMPLAZO_TLB");
    cpuconfig -> retardo_noop = config_get_int_value(iconfig, "RETARDO_NOOP");
    cpuconfig -> ip_memoria = config_get_string_value(iconfig, "IP_MEMORIA");
    cpuconfig -> puerto_memoria = config_get_string_value(iconfig, "PUERTO_MEMORIA");
    cpuconfig -> puerto_escucha_dispatch = config_get_string_value(iconfig, "PUERTO_ESCUCHA_DISPATCH");
    cpuconfig -> puerto_escucha_interrupt = config_get_string_value(iconfig, "PUERTO_ESCUCHA_INTERRUPT");
    
};