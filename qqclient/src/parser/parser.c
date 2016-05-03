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

int qq_init_connection(struct qqfs_instance *instance)
{
	struct addrinfo hints, *res;
	int sockfd;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int status = 0;
	if ((status = getaddrinfo(instance->qqserver_ip, SERVERPORT, &hints, &res)) != 0) {
		char msg[100];
		snprintf(msg, sizeof msg, "Could not getaddrinfo on qqserver_ip: %s", instance->qqserver_ip);
		qq_log_error(msg);
		return -1;
	}
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
	struct client_feedback *cf = calloc(1, sizeof(struct client_feedback));
	cf->version = PROTOCOL_VER;
	cf->sla_version = 0;
	cf->storage_id = instance->qqstorage_id;
	cf->storage_type = 0;
	cf->current_throughput = 0;
	cf->writes_queued = 0;
	cf->reads_queued = 0;
	cf->suspensions = 0;
	cf->sla_check = 0;
	cf->critical_request = 0;
	qq_send_cf(instance, cf);
	free(cf);
}

void qq_send_stat(struct qqfs_instance *instance, struct stat_info *stat)
{
	struct client_feedback *cf = calloc(1, sizeof(struct client_feedback));
	cf->version = PROTOCOL_VER;
	cf->sla_version = instance->sla_version;
	cf->storage_id = instance->qqstorage_id;
	cf->storage_type = instance->storage_type;
	cf->current_throughput = stat->iops_sec;
	cf->writes_queued = stat->writes_queued;
	cf->reads_queued = stat->reads_queued;
	cf->suspensions = stat->iops_suspended;
	cf->sla_check = 0;
	cf->critical_request = 0;
	qq_send_cf(instance, cf);
	free(cf);
}

void qq_send_cf(struct qqfs_instance *instance, struct client_feedback *cf)
{
	char *cf_json = qq_encode_cf(cf);
	int send_length = strlen(cf_json);
	int sockfd = instance->qqserver_socket;
	int sent = 0;
	do {
		int nbytes = send(sockfd, cf_json+sent, send_length-sent, 0);
		if (nbytes < 0) {
			qq_log_error("Error sending client feedback!");
			break;
		}
		sent += nbytes;
	} while (sent < send_length);
	free(cf_json);
}

void qq_close_connections()
{
	int num_servers = qqfs_instance_list->count;
	struct qqfs_instance *curr_instance = calloc(1, sizeof(struct qqfs_instance));
	for (int i = 0; i < num_servers; i++) {
		qq_get_qqfs_instance_by_idx(i, curr_instance);
		if (curr_instance->qqserver_socket > 0)
			close(curr_instance->qqserver_socket);
	}
	free(curr_instance);
}