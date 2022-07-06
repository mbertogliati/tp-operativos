#include <sockets/server_utils.h>
#include <stdbool.h>

#define TERMINAR 0 // cambienlo a lo que quieran

bool terminar() {
	int servidor = iniciar_servidor("7999");
	int cliente = esperar_cliente(servidor);
	int operacion = recibir_operacion(cliente);
	if(operacion == TERMINAR) return true;
	return false;
}
