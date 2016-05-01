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


#define MEMORY_ID_SLA "/qualiqueue/sla"
#define MEMORY_ID_STAT "/qualiqueue/stats"
#define STAT_LOCK "/qualiqueue-stat-lock"
#define SLA_LOCK "/qualiqueue-sla-lock"

#define MAX_NUM_SERVERS 5

//typedef sla_info sla_info;

typedef struct {
	int sla_id;
	int s_dev;
	int i_ino;
	//char path[80];
	int iops_min;
	int iops_max;
	int throughput_min;
	int throughput_max;
} sla_info;

typedef struct {
	sla_info slas[MAX_NUM_SERVERS];
	int count;
} sla_info_memory;

//typedef stat_info stat_info;

typedef struct {
	int s_dev;
	int i_ino;
	int iops_sec;
	int reads_queued;
	int writes_queued;
	int iops_suspended;
	int throughput;
} stat_info;

typedef struct {
	stat_info stats[MAX_NUM_SERVERS];
	char path[ 5 ][ 80 + 1 ];
	int count;
} stat_info_memory;

static sem_t *stat_lock;
static sem_t *sla_lock;

//stat_info_memory *stat_mem_info;
//sla_info_memory *sla_mem_info;

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

static void init_mem()
{
        // initialize stat memory

        key_t stat_key = ftok( MEMORY_ID_STAT, 1 );

        int shmid_stat = shmget( stat_key, sizeof( stat_info_memory ), 0600 | IPC_CREAT );
        if ( shmid_stat == -1 )
                fail( "Can't create stat shared memory" );

        stat_info_memory *stat_mem_info = (stat_info_memory *)shmat( shmid_stat, 0, 0 );
        if ( stat_mem_info == (void *)-1 )
                fail( "Can't map stat shared memory segment into address space" );

        stat_lock = sem_open( STAT_LOCK, O_CREAT, 0600, 1 );
        if ( stat_lock == SEM_FAILED )
                fail( "Can't make stat semaphore" );

        // initialize sla shared memory

        key_t sla_key = ftok( MEMORY_ID_SLA, 1 );

        int shmid_sla = shmget( sla_key, sizeof( sla_info_memory ), 0600 | IPC_CREAT );
        if ( shmid_sla == -1 )
                fail( "Can't create sla shared memory" );

        sla_info_memory *sla_mem_info = (sla_info_memory *)shmat( shmid_sla, 0, 0 );
        if ( sla_mem_info == (void *)-1 )
                fail( "Can't map sla shared memory segment into address space" );

        sla_lock = sem_open( SLA_LOCK, O_CREAT, 0600, 1 );
        if ( sla_lock == SEM_FAILED )
                fail( "Can't make sla semaphore" );

}

static void close_mem()
{
        sem_close( stat_lock );
        //shmdt( stat_mem_info );

        sem_close( sla_lock );
        //shmdt( sla_mem_info );
}
