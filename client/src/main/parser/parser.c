#include "parser.h"

void *qos_parser_start(void *in)
{
	pthread_t threads[2];
	//	find mount thread
	//		send handshake
	if (pthread_create(&threads[0], NULL, &qos_mount_listen, NULL))
		perror("Could not create mount listener thread!\n");
	//	timer thread
	//		get stats from filesystem
	//		send client feedback to qos servers
	if (pthread_create(&threads[1], NULL, &qos_feedback_timer, NULL))
		perror("Could not create feedback timer thread!\n");
	return 0;
}

void *qos_mount_listen(void *in)
{
	// listen for mounts, continuously

	// for now, just know what's happening
	
}

void *qos_feedback_timer(void *in)
{
	// timer that fires off feedback
}