#include "../include/conexion_memoria.h"

void conectar_memoria(char *ip, char *puerto) {
	memoria_log = log_create("kernel_memoria.log", "MEMORIA", true, LOG_LEVEL_INFO);
	log_info(memoria_log, "Conectando a memoria...");

	while((socket_memoria = crear_conexion(ip, puerto)) <= 0){
		log_warning(memoria_log, "No se ha podido establecer la conexion con Memoria");
		sleep(5);
	}

	int distinto_de_cero = 1;
	bool* confirmacion = malloc(sizeof(bool));
	enviar_mensaje_memoria(&distinto_de_cero, sizeof(int));

	recv(socket_memoria, confirmacion, sizeof(bool), MSG_WAITALL);

	if((*confirmacion) == false){
		log_error(memoria_log, "ERROR - Handshake fallido");
		return;
	}
	log_info(memoria_log, "Conexion con memoria exitosa");

	free(confirmacion);
}

void enviar_mensaje_memoria(void *mensaje, int tam) {
	enviar_mensaje(mensaje, tam, socket_memoria);
}

void *recibir_mensaje_memoria() {
	return recibir_mensaje(socket_memoria);
}

void desconectar_memoria() {
	liberar_conexion(socket_memoria);
}

int agregar_proceso_memoria(int pid, int tam_proceso){
	int *tabla_proceso = malloc(sizeof(int));
	log_info(logger, "Creando Proceso...");
	log_info(logger, "Enviando paquete a memoria, espero por nueva tabla");
	t_paquete *instrucciones_a_memoria = crear_paquete(10);
	agregar_a_paquete(instrucciones_a_memoria, &pid, sizeof(int));
	agregar_a_paquete(instrucciones_a_memoria, &tam_proceso, sizeof(int));
	enviar_paquete(instrucciones_a_memoria, socket_memoria);
	eliminar_paquete(instrucciones_a_memoria);

	log_info(logger, "Esperando respuesta...");
	recv(socket_memoria, tabla_proceso, sizeof(int), MSG_WAITALL);
	log_info(logger, "Respuesta Recibida!!!");
	log_info(logger, "Mi nueva tabla para el proceso %d es: %X", pid, *tabla_proceso);

	int response = *tabla_proceso;
	free(tabla_proceso);

	return response;
}

bool suspender_proceso_memoria(int direccion_tabla){
	log_info(logger, "Suspendiendo Proceso...");
	log_info(logger, "Enviando paquete a memoria, espero confirmacion de suspendimiento");
	t_paquete *instrucciones_a_memoria = crear_paquete(11);
	agregar_a_paquete(instrucciones_a_memoria, &direccion_tabla, sizeof(int));
	enviar_paquete(instrucciones_a_memoria, socket_memoria);
	eliminar_paquete(instrucciones_a_memoria);

	bool* confirmacion = malloc(sizeof(bool));

	log_info(logger, "Esperando respuesta...");
	recv(socket_memoria, confirmacion, sizeof(bool), MSG_WAITALL);
	log_info(logger, "Respuesta Recibida!!!");
	log_info(logger, "Habemos confirmacion de suspendimiento: %d", *confirmacion);

	bool retorno = *confirmacion;
	free(confirmacion);
	return retorno;
}

bool finalizar_proceso_memoria(int direccion_tabla){
	log_info(logger, "Finalizando Proceso...");
	log_info(logger, "Enviando paquete a memoria, espero confirmacion de boleteamiento");
	t_paquete *instrucciones_a_memoria = crear_paquete(16);
	agregar_a_paquete(instrucciones_a_memoria, &direccion_tabla, sizeof(int));
	enviar_paquete(instrucciones_a_memoria, socket_memoria);
	eliminar_paquete(instrucciones_a_memoria);

	bool* confirmacion = malloc(sizeof(bool));

	log_info(logger, "Esperando respuesta...");
	recv(socket_memoria, confirmacion, sizeof(bool), MSG_WAITALL);
	log_info(logger, "Respuesta Recibida!!!");
	log_info(logger, "Habemos confirmacion de boleteamiento: %d", *confirmacion);

	bool retorno = *confirmacion;
	free(confirmacion);
	return retorno;
}
