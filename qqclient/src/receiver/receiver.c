#include "receiver.h"

/**
	Listen for SLA connections to drop in from the qq server.
	Receive them, error check, and punt those babies into the kernel.
	That's it.
	Needs to be atomic.
*/

void *qq_receiver_start(void *in)
{
	qq_log_info("Starting Receiver.");
	char recvbuffer[BUFFERLENGTH];
	struct addrinfo hints, *receiver_info;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int status = getaddrinfo(NULL, LISTENPORT, &hints, &receiver_info);
	if (status != 0) {
		qq_log_error("Could not getaddrinfo.");
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}
	int receiver_socket = socket(receiver_info->ai_family, receiver_info->ai_socktype, receiver_info->ai_protocol);
	bind(receiver_socket, receiver_info->ai_addr, receiver_info->ai_addrlen);
	listen(receiver_socket, SLA_BACKLOG);
	struct sockaddr_storage server_addr;
	socklen_t addr_size;

	struct timeval stTimeOut;
	stTimeOut.tv_sec = 2;
	stTimeOut.tv_usec = 250000;

	fd_set master, read_fd;
	FD_ZERO(&master);
	FD_SET(receiver_socket, &master);

	while(check_running() != 0) {
		if (select(receiver_socket+1, &read_fd, NULL, NULL, &stTimeOut) == -1) {
			qq_log_error("Could not obtain select for socket.");
			continue;
		}
		if (!FD_ISSET(receiver_socket, &read_fd)) {
			qq_log_info("No incoming SLAs from server.\n");
			usleep(1000000);
			continue;
		}
		qq_log_info("Receiving SLA.");
		int accept_fd = accept(receiver_socket, (struct sockaddr *)&server_addr, &addr_size);
		if (recv(accept_fd, recvbuffer, BUFFERLENGTH, 0) != 0) {
			fprintf(stdout, "%s\n", recvbuffer);
			qq_log_info(recvbuffer);
			/*void *sla = qq_load_sla(recvbuffer);
			if (sla == NULL) {
				fprintf(stderr, "Invalid SLA received, failed JSON object construction.");
				continue;
			}
			if (qq_validate_sla(sla) > 0) {
				fprintf(stderr, "Invalid SLA received, failed validation.");
				continue;
			}
			// pass that ish to the kernel
			qq_release_sla(sla);*/
			close(accept_fd);
		}
	}
	freeaddrinfo(receiver_info);
	qq_log_info("Exiting Receiver.");
	return 0;
}

