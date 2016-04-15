#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <semaphore.h>

#include "common.c"

#define MEMORY_ID_SLA "/qualiqueue/sla"
#define MEMORY_ID_STAT "/qualiqueue/stats"
#define STAT_LOCK "/qualiqueue-stat-lock"
#define SLA_LOCK "/qualiqueue-sla-lock"

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

static sem_t *stat_lock;
static sem_t *sla_lock;

stat_info_memory *stat_info;
sla_info_memory *sla_info;