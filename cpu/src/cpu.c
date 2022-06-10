#include "../include/cpu.h"

int main(void){

    t_config_cpu *configuracion_cpu;
    configuracion_cpu = cargar_config("config/cpu.config");
    TLB = list_create();
    iniciar_conexiones();

    free(cpuconfig);
    log_destroy(memoria_log);
    log_destroy(cpu_log);
};