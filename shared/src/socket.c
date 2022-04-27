#include "../include/socket.h"
#include <string.h>

struct addrinfo *addrinfo_servidor(char *ip, char *puerto) {
	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	return server_info;
}

int crear_socket(struct addrinfo *server_info) {
	return socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
}
