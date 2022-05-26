#include ".../include/estructurascpu.h" 

t_config_cpu *cargar_config (char** path){
    t_config *config_cpu = NULL;
    t_config_cpu *cpuconfig;

    cpu_log = log_create("logs/cpu.log","CPU",true,LOG_LEVEL_INFO);

    if(!(config_cpu = config_create("config/cpu.config"))){
        log_error(cpu_log,"ERROR - No se encontro el archivo de configuracion de CPU");
        return NULL;
    }

   if (! es_configuracion_valida(config_cpu))
    return NULL;

    cpuconfig -> entradas_tlb = config_get_int_value(iconfig, "ENTRADAS_TLB");
    cpuconfig -> reemplazo_tlb = config_get_string_value(iconfig, "REEMPLAZO_TLB");
    cpuconfig -> retardo_noop = config_get_int_value(iconfig, "RETARDO_NOOP");
    cpuconfig -> ip_memoria = config_get_string_value(iconfig, "IP_MEMORIA");
    cpuconfig -> puerto_memoria = config_get_string_value(iconfig, "PUERTO_MEMORIA");
    cpuconfig -> puerto_escucha_dispatch = config_get_string_value(iconfig, "PUERTO_ESCUCHA_DISPATCH");
    cpuconfig -> puerto_escucha_interrupt = config_get_string_value(iconfig, "PUERTO_ESCUCHA_INTERRUPT");
    
    return config_cpu;
   
}

bool es_configuracion_valida(t_config *config_cpu){
    char claves_validas[7][20+1] = {"ENTRADAS_TLB", "REEMPLAZO_TLB", "RETARDO NOOP", "IP_MEMORIA", 
                                        "PUERTO_MEMORIA", "PUERTO_ESCUCHA_DISPATCH", "PUERTO_ESCUCHA_INTERRUPT"};
                                        
    for(int i = 0; i < 0; i++){
        if (!config_has_property(config_cpu, claves_validas[i])){
            log_error(memoria_log,"ERROR - Archivo de configuracion invalido");
            return false;
        }
    }
    if (config_keys_amount(config_cpu) >7 ){
        log_error(memoria_log,"ERROR - Archivo de configuracion invalido");
        return false;
    }
    return true;
}