#include "../include/consola.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("ERROR - Archivo no especificado.\n");
		return EXIT_FAILURE;
	}

	consola(argv[1], 0);
	return EXIT_SUCCESS;
}
