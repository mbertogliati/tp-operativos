#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void consola_interactiva() {
	char *linea;

	puts("Leyendo líneas (exit para salir)\n"
		 "--------------------------------\n");

	while (1) {
		linea = readline("> ");
		if (linea)
			add_history(linea);
		if (!strncmp(linea, "exit", 4)) {
			free(linea);
			break;
		}
		printf("%s\n", linea);
		free(linea);
	}

	HISTORY_STATE *myhist = history_get_history_state();
	HIST_ENTRY **mylist = history_list();

	puts("\nLíneas ingresadas al historial:\n");
	for (int i = 0; i < myhist->length; i++) {
		printf("-\t%s\n", mylist[i]->line);
		free_history_entry(mylist[i]);
	}
	putchar('\n');

	free(myhist);
	free(mylist);
}
