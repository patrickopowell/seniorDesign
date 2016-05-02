#include "parser.h"

void *qq_parser_start(void *in)
{
	qq_log_info("Starting Parser.");
	while(check_running() != 0) {
		usleep(1000000);
		qq_lock();
		int num_servers = qqfs_instance_list->count;
		qq_unlock();
		struct qqfs_instance *curr_instance = calloc(1, sizeof(struct qqfs_instance));
		struct stat_info *curr_stat = calloc(1, sizeof(struct stat_info));
		for (int i = 0; i < num_servers; i++) {
			qq_get_qqfs_instance_by_idx(i, curr_instance)
			com_get_stat(curr_instance->export_path, curr_stat);
			qq_parse_stat(&curr_stat);
		}
		free(curr_instance);
		free(curr_stat);
	}
	qq_log_info("Exiting Parser.");
	return 0;
}

void qq_parse_stat(struct stat_info *stat)
{
	printf("%d %d\n%s\n%d %d %d %d %d", stat->s_dev, stat->i_ino, stat->path, stat->iops_sec, stat->reads_queued, stat->writes_queued, stat->iops_suspended, stat->throughput);
}

/**
 * Construct handshake to send to qq server.
 * Pass pointer to handshake as return.
 * void pointer type just in case we need to change return type to
 * something like a string, currently returns client_feedback struct.
 * To change, just code in and change the cast.
 */
void *qq_construct_handshake()
{
	client_feedback *handshake = calloc(1, sizeof(client_feedback));
	handshake->version = 0;
	handshake->sla_version = 0;
	handshake->storage_id.s_dev = 0;
	handshake->storage_id.i_ino = 0;
	handshake->storage_type = 0;
	handshake->current_throughput = 0;
	handshake->writes_queued = 0;
	handshake->reads_queued = 0;
	handshake->suspensions = 0;
	handshake->sla_check = 0;
	handshake->critical_request = 0;
	return handshake;
}