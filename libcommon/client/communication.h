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
#include "../logging.h"

#define COM_SLA_MEM "/qualiqueue/sla"
#define COM_STAT_MEM "/qualiqueue/stats"
#define COM_STAT_LOCK "/qualiqueue-stat-lock"
#define COM_SLA_LOCK "/qualiqueue-sla-lock"

#define COM_STAT_CREATE_FAIL -1
#define COM_STAT_MAP_FAIL -2
#define COM_STAT_SEM_FAIL -3
#define COM_SLA_CREATE_FAIL -4
#define COM_SLA_MAP_FAIL -5
#define COM_SLA_SEM_FAIL -6
#define COM_ELEMENT_NFOUND -7
#define COM_MEM_OOS -8

#define COM_MAX_SERVERS 5

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
	struct sla_info slas[COM_MAX_SERVERS];
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
	struct stat_info stats[COM_MAX_SERVERS];
	int count;
};

struct stat_info_memory *com_stat_list;
struct sla_info_memory *com_sla_list;

int com_init_mem();
void com_close_mem();
int com_init_stat_mem();
int com_init_stat_sem();
void com_lock_stat();
void com_unlock_stat();
int com_init_sla_mem();
int com_init_sla_sem();
void com_lock_sla();
void com_unlock_sla();
void com_close_stat();
void com_close_sla();
int com_get_sla(char *path, struct sla_info *sla_dest);
int com_set_sla(char *path, struct sla_info *sla_src);
int com_get_stat(char *path, struct stat_info *stat_dest);
int com_set_stat(char *path, struct stat_info *stat_src);