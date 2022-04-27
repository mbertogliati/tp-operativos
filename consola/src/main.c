#include "../include/consola.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	if(!son_argumentos_validos(argc, argv))
		return EXIT_FAILURE;
	
	int tamanio = atoi(argv[2]);
	
	consola(argv[1], tamanio);

	return EXIT_SUCCESS;
}
