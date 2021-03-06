/**
 * QualiQueue - Spring 2016
 * @author Remington Campbell <racampbe@ncsu.edu
 *
 * Definition of common protocol used between
 * QQClient and QQServer for communciation.
 */
#ifndef PROTOCOL
#define PROTOCOL_VER 1
struct client_feedback {
	int version;
	int sla_version;
	int storage_id;
	int storage_type;
	int current_throughput;
	int writes_queued;
	int reads_queued;
	int suspensions;
	int sla_check;
	int critical_request;
};

struct sla {
	int version;
	int sla_version;
	char *client_id;
	int storage_id;
	int storage_type;
	int iops_max;
	int iops_min;
	int throughput_min;
	int throughput_max;
	int unused_bandwidth;
};
#define PROTOCOL
#endif