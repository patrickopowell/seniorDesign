#include "parser.h"

void *qq_parser_start(void *in)
{
	qq_log_info("Starting Parser.");
	struct qqfs_instance *curr_instance = calloc(1, sizeof(struct qqfs_instance));
	struct stat_info *curr_stat = calloc(1, sizeof(struct stat_info));
	while(check_running() != 0) {
		usleep(1000000);
		qq_lock();
		int num_servers = qqfs_instance_list->count;
		qq_unlock();
		for (int i = 0; i < num_servers; i++) {
			qq_get_qqfs_instance_by_idx(i, curr_instance);
			if (curr_instance->qqserver_socket <= 0) {
				if (qq_init_connection(curr_instance) > 0)
					qq_send_handshake(curr_instance);
			}
			if (com_get_stat(curr_instance->export_path, curr_stat) < 0)
				continue;
			qq_send_stat(curr_instance, curr_stat);
		}
	}
	free(curr_instance);
	free(curr_stat);
	qq_log_info("Exiting Parser.");
	qq_close_connections();
	return 0;
}

void qq_parse_stat(struct stat_info *stat)
{
	printf("%s\n%d %d %d %d %d\n", stat->path, stat->iops_sec, stat->reads_queued, stat->writes_queued, stat->iops_suspended, stat->throughput);
}

int qq_init_connection(struct qqfs_instance *instance)
{
	struct addrinfo hints, *res;
	int sockfd;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(instance->qqserver_ip, SERVERPORT, &hints, &res);
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
		qq_log_error("Could not connect to specified qqserver! Will retry on next pass.");
		qq_log_error(strerror(errno));
	} else
		instance->qqserver_socket = sockfd;
	return sockfd;
}

void qq_send_handshake(struct qqfs_instance *instance)
{

}

void qq_send_stat(struct qqfs_instance *instance, struct stat_info *stat)
{

}

void qq_close_connections()
{
	qq_lock();
	int num_servers = qqfs_instance_list->count;
	struct qqfs_instance *curr_instance = calloc(1, sizeof(struct qqfs_instance));
	for (int i = 0; i < num_servers; i++) {
		qq_get_qqfs_instance_by_idx(i, curr_instance);
		if (curr_instance->qqserver_socket > 0)
			close(curr_instance->qqserver_socket);
	}
	free(curr_instance);
	qq_unlock();
}