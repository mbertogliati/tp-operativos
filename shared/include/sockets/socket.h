#ifndef SOCKET_H_
#define SOCKET_H_

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum {
	INSTRUCCIONES_CONSOLA, MENSAJE
} op_code;

struct addrinfo *addrinfo_servidor(char *ip, char *puerto);
int crear_socket(struct addrinfo *server_info);

#endif /* SOCKET_H_ */
