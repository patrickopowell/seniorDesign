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

//#include "common.c"

#define MEMORY_ID_SLA "/qualiqueue/sla"
#define MEMORY_ID_STAT "/qualiqueue/stats"
#define STAT_LOCK "/qualiqueue-stat-lock"
#define SLA_LOCK "/qualiqueue-sla-lock"

#define MAX_NUM_SERVERS 5

//typedef sla_info sla_info;

struct sla_info {
	int sla_id;
	int s_dev;
	int i_ino;
	int iops_min;
	int iops_max;
	int throughput_min;
	int throughput_max;
};

struct sla_info_memory {
	struct sla_info slas[MAX_NUM_SERVERS];
	int count;
};

//typedef stat_info stat_info;

struct stat_info {
	int s_dev;
	int i_ino;
	int iops_sec;
	int reads_queued;
	int writes_queued;
	int iops_suspended;
	int throughput;
};

struct stat_info_memory {
	struct stat_info stats[MAX_NUM_SERVERS];
	int count;
};

static sem_t *stat_lock;
static sem_t *sla_lock;

struct stat_info_memory *stat_mem_info;
struct sla_info_memory *sla_mem_info;

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

void init_mem()
{
	// initialize stat memory
	
	key_t stat_key = ftok( MEMORY_ID_STAT, 1 );

	int shmid_stat = shmget( stat_key, sizeof( stat_info_memory ), 0600 | IPC_CREAT );
	if ( shmid_stat == -1 )
		fail( "Can't create shared memory" );

	stat_mem_info = (struct stat_info_memory *)shmat( shmid, 0, 0 );
	if ( stat_mem_info == (void *)-1 )
		fail( "Can't map shared memory segment into address space" );

	stat_lock = sem_open( STAT_LOCK, O_CREAT, 0600, 1 );
	if ( stat_lock == SEM_FAILED )
		fail( "Can't make semaphore" );
	
	// initialize sla shared memory
	
	key_t sla_key = ftok( MEMORY_ID_SLA, 1 );

	int shmid_sla = shmget( sla_key, sizeof( sla_info_memory ), 0600 | IPC_CREAT );
	if ( shmid_sla == -1 )
		fail( "Can't create shared memory" );

	sla_mem_info = (struct sla_info_memory *)shmat( shmid, 0, 0 );
	if ( sla_mem_info == (void *)-1 )
		fail( "Can't map shared memory segment into address space" );

	sla_lock = sem_open( SLA_LOCK, O_CREAT, 0600, 1 );
	if ( sla_lock == SEM_FAILED )
		fail( "Can't make semaphore" );
}

void close_mem()
{
	sem_close( stat_lock );
	shmdt( stat_mem_info );
	
	sem_close( sla_lock );
	shmdt( sla_mem_info );
}