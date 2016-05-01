#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <semaphore.h>
#include "../lib/logging/logging.h"

#define SHR_SLA_MEM "/qualiqueue/sla"
#define SHR_STAT_MEM "/qualiqueue/stats"
#define SHR_STAT_LOCK "/qualiqueue-stat-lock"
#define SHR_SLA_LOCK "/qualiqueue-sla-lock"

#define SHR_STAT_CREATE_FAIL -1
#define SHR_STAT_MAP_FAIL -2
#define SHR_STAT_SEM_FAIL -3
#define SHR_SLA_CREATE_FAIL -4
#define SHR_SLA_MAP_FAIL -5
#define SHR_SLA_SEM_FAIL -6

#define SHR_MAX_SERVERS 5

struct sla_info {
	int sla_id;
	int s_dev;
	int i_ino;
	char path[80];
	int iops_min;
	int iops_max;
	int throughput_min;
	int throughput_max;
};

struct sla_info_memory {
	struct sla_info slas[SHR_MAX_SERVERS];
	int count;
};

struct stat_info {
	int s_dev;
	int i_ino;
	char path[80];
	int iops_sec;
	int reads_queued;
	int writes_queued;
	int iops_suspended;
	int throughput;
};

struct stat_info_memory {
	struct stat_info stats[SHR_MAX_SERVERS];
	int count;
};

struct stat_info_memory *shr_stat_list;
struct sla_info_memory *shr_sla_list;

int shr_init_mem();
void shr_close_mem();
int shr_init_stae_mem();
int shr_init_stat_sem();
void shr_lock_stat();
void shr_unlock_stat();
int shr_init_sla_mem();
int shr_init_sla_sem();
void shr_lock_sla();
void shr_unlock_sla();
void shr_close_stat();
void shr_close_sla();
