#include "parser.h"

void *qos_parser_start(void *in)
{
	pthread_t threads[1];
	//	timer thread
	//		get stats from filesystem
	//		send client feedback to qos servers
	if (pthread_create(&threads[1], NULL, &qos_feedback_timer, NULL))
		perror("Could not create feedback timer thread!\n");
	for (int i = 0; i < RESPONSIBILITIES; i++)
		pthread_join(threads[i], NULL);
	printf("Exiting Parser Thread.\n");
	return 0;
}

void qos_send_feedback(client_feedback *cf)
{
	char *server_qos_ip = get_qos_ip(cf->s_dev, cf->i_ino);
	char *fb = qos_obj_to_string(qos_construct_client_feedback(cf));
}

char *get_qos_ip(int s_dev, int i_ino)
{
	if (server_list == NULL)
		return NULL;
	storage_server *s = server_list;
	while (s != NULL) {
		if (s->s_dev == s_dev && s->i_ino == i_ino)
			return s->qos_ip;
		s = s->next;
	}
	return NULL;
}

/**
 * Construct handshake to send to QoS server.
 * Pass pointer to handshake as return.
 * void pointer type just in case we need to change return type to
 * something like a string, currently returns client_feedback struct.
 * To change, just code in and change the cast.
 */
void *qos_construct_handshake()
{
	client_feedback *handshake = calloc(sizeof client_feedback, 0);
	handshake->version = 0;
	handshake->sla_version = 0;
	handshake->storage_id->s_dev = 0;
	handshake->storage_id->i_ino = 0;
	handshake->storage_type = 0;
	handshake->current_throughput = 0;
	handshake->writes_queued = 0;
	handshake->reads_queued = 0;
	handshake->suspensions = 0;
	handshake->sla_check = 0;
	handshake->critical_request = 0;
	return handshake;
}

void *qos_feedback_timer(void *in)
{
	while(check_running() != 0)
		usleep(1000000);
}