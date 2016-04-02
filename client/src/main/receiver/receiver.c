#include "receiver.h"

/**
	Listen for SLA connections to drop in from the QoS server.
	Receive them, error check, and punt those babies into the kernel.
	That's it.
	Needs to be atomic.
*/

int receiver_start()
{
	char recvbuffer[BUFFERLENGTH];
	int sockfd;
	int status;
	struct addrinfo hints, *receiver_info;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, SERVERPORT, &hints, &receiver_info)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}
	int receiver_socket = socket(receiver_info->ai_family, receiver_info->ai_socktype, receiver_info->ai_protocol);
	bind(receiver_socket, receiver_info->ai_addr, receiver_info->ai_addrlen);
	listen(receiver_socket, SLA_BACKLOG);
	struct sockaddr_storage server_addr;
	socklen_t addr_size;
	accept_fd = accept(receiver_socket, (struct sockaddr *)&server_adr, &addr_size);
	freeaddrinfo(servinfo);
}

