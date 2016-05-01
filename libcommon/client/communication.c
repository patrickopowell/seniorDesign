#include "shared_common.h"

static key_t stat_key;
static key_t sla_key;

static sem_t *stat_lock;
static sem_t *sla_lock;

int com_init_mem()
{
        int stat_mem_status = com_init_stat_mem();
        if (stat_mem_status < 0) {
                qq_log_critical("Could not create stat shared memory!");
                return stat_mem_status;
        }

        int stat_sem_status = com_init_stat_sem();
        if (stat_sem_status < 0) {
                qq_log_critical("Could not open stat semaphore!");
                return stat_sem_status;
        }

        int sla_mem_status = com_init_sla_mem();
        if (sla_mem_status < 0) {
                qq_log_critical("Could not create sla shared memory!");
                return sla_mem_status;
        }

        int sla_sem_status = com_init_sla_sem();
        if (sla_sem_status < 0) {
                qq_log_critical("Could not open sla semaphore!");
                return sla_sem_status;
        }
}

void com_close_mem()
{
        com_close_stat();
        com_close_sla();
}

int com_init_stat_mem()
{
        stat_key = ftok(COM_STAT_MEM, 1);
        int shmid_stat = shmget(stat_key, sizeof(struct stat_info_memory), 0600 | IPC_CREAT);
        if (shmid_stat == -1) {
                qq_log_critical("Can't create stat shared memory!");
                return COM_STAT_CREATE_FAIL;
        } else {
                com_stat_list = (struct stat_info_memory *)shmat(shmid_stat, 0, 0);
                if (com_stat_list == (void *)-1) {
                        qq_log_critical("Can't map stat shared memory into address space!");
                        return COM_STAT_MAP_FAIL;
                }
        }
        return 0;
}

int com_init_stat_sem()
{
        stat_lock = sem_open(COM_STAT_LOCK, O_CREAT, 0600, 1);
        if (stat_lock == SEM_FAILED) {
                qq_log_critical("Stat semaphore creation failed!");
                return COM_STAT_SEM_FAIL;
        }
        return 0;
}

void com_lock_stat()
{
        sem_wait(stat_lock);
}

void com_unlock_stat()
{
        sem_post(stat_lock);
}

int com_init_sla_mem()
{
        sla_key = ftok(COM_SLA_MEM, 1);
        int shmid_sla = shmget(sla_key, sizeof(struct sla_info_memory), 0600 | IPC_CREAT);
        if (shmid_sla == -1) {
                qq_log_critical("Can't create SLA shared memory!");
                return COM_SLA_CREATE_FAIL;
        } else {
                com_sla_list = (struct sla_info_memory *)shmat(shmid_sla, 0, 0);
                if (com_sla_list == (void *)-1) {
                        qq_log_critical("Can't map SLA shared memory into address space!");
                        return COM_SLA_MAP_FAIL;
                }
        }
        return 0;
}

int com_init_sla_sem()
{
        sla_lock = sem_open(COM_SLA_LOCK, O_CREAT, 0600, 1);
        if (sla_lock == SEM_FAILED) {
                qq_log_critical("SLA semaphore creation failed!");
                return COM_SLA_SEM_FAIL;
        }
        return 0;
}

void com_lock_sla()
{
        sem_wait(sla_lock);
}

void com_unlock_sla()
{
        sem_post(sla_lock);
}

void com_close_stat()
{
        sem_close(stat_lock);
        shmdt(com_stat_list);
}

void com_close_sla()
{
        sem_close(sla_lock);
        shmdt(com_sla_list);
}