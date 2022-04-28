#ifndef SOCKET_H_
#define SOCKET_H_

#include <netdb.h>
#include <sys/socket.h>

#define IP "127.0.0.1"
#define PUERTO "8000"

typedef enum {
	INSTRUCCIONES_CONSOLA
} op_code;

struct addrinfo *addrinfo_servidor(char *ip, char *puerto);
int crear_socket(struct addrinfo *server_info);

#endif /* SOCKET_H_ */
