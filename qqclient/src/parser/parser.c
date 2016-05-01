#include "parser.h"

void *qq_parser_start(void *in)
{
	qq_log_info("Starting Parser.");
	while(check_running() != 0)
		usleep(1000000);
	qq_log_info("Exiting Parser.");
	return 0;
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