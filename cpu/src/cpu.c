#include "../include/cpu.h"

int main(void){
	t_config *config = cargar_config("config/cpu.config");
    TLB = list_create();
    iniciar_conexiones();
    config_destroy(config);
    log_destroy(memoria_log);
    log_info(cpu_log, "El CPU finalizo correctamente");
    log_destroy(cpu_log);
};
