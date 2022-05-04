#include "../include/conexiones.h"

int esKernel(int socket_cliente){
    uint8_t recibido;
    recibido = recv(socket_cliente,&recibido,sizeof(uint8_t),MSG_WAITALL);
    send(socket_cliente,&recibido,sizeof(uint8_t),0);
    if (recibido == 0)
        return true;
    
    return false;
}

int conectar_con_kernel(t_config_memoria *configs, int socket_cliente){
	return 1;
}

int conectar_con_cpu(t_config_memoria *configs, int socket_cliente){
	return 1;
}

int iniciar_conexion(t_config_memoria *configs){
//    int socket_servidor;
//    socket_servidor = iniciar_servidor(configs->puerto);
//
//    int socket_cliente;
//
//    while(true){
//        socket_cliente = esperar_cliente(socket_servidor);
//
//        if (esKernel(socket_cliente)){
//            conectar_con_kernel(configs,socket_cliente);
//        }
//        else conectar_con_cpu(configs,socket_cliente);
//    }
	return 1;
}
