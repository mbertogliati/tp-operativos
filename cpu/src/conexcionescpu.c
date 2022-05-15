#include "../include/conexionescpu.h"


void iniciar_conexion_memoria (){

    int socket_cpu;
    void* buffer;

    socket_cpu = crear_conexion();  //no se como llamar a mi estructura
        
    buffer = recibir_buffer(); 
    
    // tengo que hacer un recieve y crear un buffer

};

void iniciar_conexion_dispatch(){
    //socket servidor
    int socket_dispatch; 
    socket_dispatch = iniciar_servidor();

};

void iniciar_conexion_interrupt(){
    //socket servidor 
    int socket_interrupt;
    socket_interrupt = iniciar_servidor();

};
