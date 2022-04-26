#include "../include/consola.h"
#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>

t_log* log_consola;

void consola(char *path, int tamanio) {
	log_consola = log_create("consola/consola.log", "CONSOLA", true, LOG_LEVEL_INFO);

	t_list *lista_de_instrucciones = leer_archivo(path);

	//tamanio = list_size(lista_de_instrucciones) * sizeof(t_link_element);

	t_paquete *paquete = crear_paquete_instrucciones(lista_de_instrucciones->head, tamanio);
	enviar_paquete_instrucciones(paquete);
	liberar_lista(lista_de_instrucciones);
}

t_list *leer_archivo(char *path) {
	FILE *f = fopen(path, "r");

	if (!f) {
		log_error(log_consola, "No se encontró el archivo de instrucciones");
		return NULL;
	}

	char *linea = NULL;
	size_t lon = 0;
	t_list *lista_de_instrucciones = list_create();

	while (getline(&linea, &lon, f) != -1)
		procesar_linea(string_split(linea, " "), lista_de_instrucciones);

	fclose(f);
	free(linea);

	log_info(log_consola, "Archivo de instrucciones leído correctamente");
	log_info(log_consola, "Lista de instrucciones creada");
	//leer_lista(lista_de_instrucciones);
	return lista_de_instrucciones;
}

t_paquete *crear_paquete_instrucciones(t_link_element *lista_de_instrucciones, int tamanio) {
	log_info(log_consola, "Creando paquete de instrucciones...");

	t_paquete *paquete = crear_paquete();
	agregar_a_paquete(paquete, lista_de_instrucciones, tamanio);

	log_info(log_consola, "Paquete creado exitosamente");

	return paquete;
}

void enviar_paquete_instrucciones(t_paquete *paquete) {
	log_info(log_consola, "Iniciando conexión con Kernel...");

	int socket_cliente = conectar_a_kernel();

	if (socket_cliente < 0) {
		log_error(log_consola, "No se ha podido establecer comunicación con el Kernel");
		return;
	}

	enviar_paquete(paquete, socket_cliente);
	eliminar_paquete(paquete);
	liberar_conexion(socket_cliente);
}

int conectar_a_kernel() {
	t_config *config = config_create("consola/consola.config");
	char *ip, *puerto;

	if (!config) {
		log_error(log_consola, "No se encontró el archivo de configuracion de Kernel");
		return -1;
	}

	if (config_has_property(config, "IP_KERNEL") && config_has_property(config, "PUERTO_KERNEL")) {
		ip = config_get_string_value(config, "IP_KERNEL");
		puerto = config_get_string_value(config, "PUERTO_KERNEL");
	}
	else {
		log_error(log_consola, "El archivo de configuración de Kernel es erróneo");
		return -1;
	}

	config_destroy(config);

	return crear_conexion(ip, puerto);
}
