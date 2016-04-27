#include "receiver.h"

/**
	Listen for SLA connections to drop in from the QoS server.
	Receive them, error check, and punt those babies into the kernel.
	That's it.
	Needs to be atomic.
*/

void *qos_receiver_start(void *in)
{
	char recvbuffer[BUFFERLENGTH];
	int status;
	struct addrinfo hints, *receiver_info;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, LISTENPORT, &hints, &receiver_info)) != 0) {
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
			perror("Could not obtain select for socket.\n");
			continue;
		}
		if (!FD_ISSET(receiver_socket, &read_fd)) {
			perror("No incoming SLAs from server.\n");
			usleep(1000000);
			continue;
		}
		printf("SLA incoming!\n");
		int accept_fd = accept(receiver_socket, (struct sockaddr *)&server_addr, &addr_size);
		if (recv(accept_fd, recvbuffer, BUFFERLENGTH, 0) != 0) {
			fprintf(stdout, "%s\n", recvbuffer);
			void *sla = qos_load_sla(recvbuffer);
			if (sla == NULL) {
				fprintf(stderr, "Invalid SLA received, failed JSON object construction.");
				continue;
			}
			if (qos_validate_sla(sla) > 0) {
				fprintf(stderr, "Invalid SLA received, failed validation.");
				continue;
			}
			// pass that ish to the kernel
			qos_release_sla(sla);
		}
		close(accept_fd);
	}
	freeaddrinfo(receiver_info);
}

void qos_new_mgmt(char *ip, int s_dev, int i_ino)
{
	storage_server *s = get_last_mgmt_server();
	storage_server *new_s = calloc(sizeof storage_server, 0);
	new_s->s_dev = s_dev;
	new_s->i_ino = i_ino;
	strcpy(new_s->qos_ip, ip);
	new_s->connection_fd = generate_mgmt_connection(ip);
	s->next = new_s;
}

storage_server *get_last_mgmt_server() {
	if (server_list == NULL)
		return NULL;
	storage_server *s = server_list;
	while (s->next != NULL)
		s = s->next;
	return s;
}

int generate_mgmt_connection(char *ip)
{
	struct addrinfo hints, *receiver_info;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, LISTENPORT, &hints, &receiver_info)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}
	int receiver_socket = socket(receiver_info->ai_family, receiver_info->ai_socktype, receiver_info->ai_protocol);
	return receiver_socket;
}

