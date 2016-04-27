#define MEMORY_ID_SLA "/qualiqueue/sla"
#define MEMORY_ID_STAT "/qualiqueue/stats"

#define MAX_NUM_SERVERS 5

typedef sla_info sla_info;

struct sla_info {
	int sla_id;
	int s_dev;
	int i_ino;
	int iops_min;
	int iops_max;
	int throughput_min;
	int throughput_max;
};

struct {
	sla_info slas[MAX_NUM_SERVERS];
	int count;
} sla_info_memory;

typedef stat_info stat_info;

struct stat_info {
	int s_dev;
	int i_ino;
	int iops_sec;
	int reads_queued;
	int writes_queued;
	int iops_suspended;
	int throughput;
};

struct {
	stat_info stats[MAX_NUM_SERVERS];
	int count;
} stat_info_memory;

/*
Isn't actually how it'll work but you get the idea.
Flow:
Mount filesystem
BBFS Mount that folder
	from mount get storage identifiers
	named semaphore with storage Identifier
	ftok shared memory with storage Identifier
		create shared memory
Client detects bbfs mount
	from mount get storage identifiers
	do client stuff
*/