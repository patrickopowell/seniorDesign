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

	while (check_running() != 0) {
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
		int received = 0;
		do {
			int nbytes = recv(accept_fd, recvbuffer+received, BUFFERLENGTH-received, 0);
			if (nbytes == 0)
				break;
			received += nbytes;
		} while (received < BUFFERLENGTH);
		close(accept_fd);

		/** Spaghetti monster. **/

		struct sockaddr_in addr;
		getpeername(accept_fd, (struct sockaddr *)&addr, (socklen_t *)sizeof(struct sockaddr_in));
		char *qqserver_ip = inet_ntoa(addr.sin_addr);
		struct sla *new_sla = calloc(1, sizeof(struct sla));

		qq_decode_sla(recvbuffer, new_sla);

		struct qqfs_instance *curr_instance = calloc(1, sizeof(struct qqfs_instance));
		qq_get_qqfs_instance_by_pair(qqserver_ip, new_sla->storage_id, curr_instance);

		struct sla_info *sla_comm = calloc(1, sizeof(struct sla_info));
		memcpy(curr_instance->export_path, sla_comm->path, sizeof curr_instance->export_path);
		sla_comm->sla_id = new_sla->sla_version;
		sla_comm->iops_min = new_sla->iops_min;
		sla_comm->iops_max = new_sla->iops_max;
		sla_comm->throughput_min = new_sla->throughput_min;
		sla_comm->throughput_max = new_sla->throughput_max;
		com_set_sla(curr_instance->export_path, sla_comm);

		curr_instance->sla_version = new_sla->sla_version;
		curr_instance->storage_type = new_sla->storage_type;
		qq_update_qqfs_instance(curr_instance);

		free(sla_comm);
		free(curr_instance);
		free(new_sla);
	}
	freeaddrinfo(receiver_info);
	qq_log_info("Exiting Receiver.");
	return 0;
}

