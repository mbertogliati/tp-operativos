#include "../include/consola.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc < 3) {
		printf("ERROR - Archivo no especificado.\n");
		return EXIT_FAILURE;
	}

	int tamanio = atoi(argv[2]);

	if (tamanio)
		consola(argv[1], tamanio);

	return EXIT_SUCCESS;
}
