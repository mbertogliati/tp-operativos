#include "../../shared/include/server_utils.h"
#include "../include/kernel.h"

int main() {
	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

	t_paquete_consola* paquete_consola;

	while (1) {
		int cod_op = recibir_operacion(cliente_fd);

		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;

		case INSTRUCCIONES_CONSOLA:

			paquete_consola = recibir_paquete_instrucciones_consola(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			leer_lista(paquete_consola->lista_de_instrucciones);

			log_info(logger, "El proceso ocupara:\n");
			printf("%d Bytes de Memoria\n", paquete_consola->memory_size);

			liberar_lista(paquete_consola->lista_de_instrucciones);
			free(paquete_consola);
			break;

		case -1:
			log_error(logger, "El cliente se desconectó. Terminando servidor");
			return EXIT_FAILURE;

		default:
			log_warning(logger, "Operación desconocida.");
			break;
		}
	}

	log_destroy(logger);
	return EXIT_SUCCESS;
}
/*
	Cree una nueva estructura que tiene la lista + el tamaño que se ingreso por consola
	solo para no hacerme lio, si queres despues lo podes cambiar
*/
t_paquete_consola *recibir_paquete_instrucciones_consola(int socket_cliente) {
	int size, tamanio_en_memoria, desplazamiento = 0;
	void *buffer;

	buffer = recibir_buffer(&size, socket_cliente);
	
	tamanio_en_memoria = recibir_tamanio_en_memoria(buffer);
	desplazamiento += sizeof(int);


	t_paquete_consola *paquete_recibido = malloc(sizeof(t_paquete_consola));
	t_list *instrucciones = list_create();
	t_instruccion *instruccion;

	while (desplazamiento < size) {
		int identificador, cant_parametros;
		uint32_t *parametros;

		memcpy(&identificador, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(&cant_parametros, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		if (cant_parametros) {
			int tam_cant_parametros = cant_parametros * sizeof(uint32_t);
			parametros = (uint32_t *) malloc(tam_cant_parametros);

			memcpy(parametros, buffer + desplazamiento, tam_cant_parametros);
			desplazamiento += tam_cant_parametros;
		}
		else parametros = NULL;

		instruccion = crear_instruccion(identificador, cant_parametros, parametros);
		list_add(instrucciones, instruccion);
	}
	free(buffer);
	paquete_recibido->lista_de_instrucciones = instrucciones;
	paquete_recibido->memory_size =	tamanio_en_memoria;
	
	return paquete_recibido;
}
