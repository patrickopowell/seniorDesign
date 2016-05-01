#include "shared_common.h"

static key_t stat_key;
static key_t sla_key;

static sem_t *stat_lock;
static sem_t *sla_lock;

int shr_init_mem()
{
        int stat_mem_status = shr_init_stat_mem();
        if (stat_mem_status < 0) {
                qos_log_critical("Could not create stat shared memory!");
                return stat_mem_status;
        }

        int stat_sem_status = shr_init_stat_sem();
        if (stat_sem_status < 0) {
                qos_log_critical("Could not open stat semaphore!");
                return stat_sem_status;
        }

        int sla_mem_status = shr_init_sla_mem();
        if (sla_mem_status < 0) {
                qos_log_critical("Could not create sla shared memory!");
                return sla_mem_status;
        }

        int sla_sem_status = shr_init_sla_sem();
        if (sla_sem_status < 0) {
                qos_log_critical("Could not open sla semaphore!");
                return sla_sem_status;
        }
}

void shr_close_mem()
{
        shr_close_stat();
        shr_close_sla();
}

int shr_init_stat_mem()
{
        stat_key = ftok(SHR_STAT_MEM, 1);
        int shmid_stat = shmget(stat_key, sizeof(struct stat_info_memory), 0600 | IPC_CREAT);
        if (shmid_stat == -1) {
                qos_log_critical("Can't create stat shared memory!");
                return SHR_STAT_CREATE_FAIL;
        } else {
                shr_stat_list = (struct stat_info_memory *)shmat(shmid_stat, 0, 0);
                if (shr_stat_list == (void *)-1) {
                        qos_log_critical("Can't map stat shared memory into address space!");
                        return SHR_STAT_MAP_FAIL;
                }
        }
        return 0;
}

int shr_init_stat_sem()
{
        stat_lock = sem_open(SHR_STAT_LOCK, O_CREAT, 0600, 1);
        if (stat_lock == SEM_FAILED) {
                qos_log_critical("Stat semaphore creation failed!");
                return SHR_STAT_SEM_FAIL;
        }
        return 0;
}

void shr_lock_stat()
{
        sem_wait(stat_lock);
}

void shr_unlock_stat()
{
        sem_post(stat_lock);
}

int shr_init_sla_mem()
{
        sla_key = ftok(SHR_SLA_MEM, 1);
        int shmid_sla = shmget(sla_key, sizeof(struct sla_info_memory), 0600 | IPC_CREAT);
        if (shmid_sla == -1) {
                qos_log_critical("Can't create sla shared memory!");
                return SHR_SLA_CREATE_FAIL;
        } else {
                shr_sla_list = (struct sla_info_memory *)shmat(shmid_stat, 0, 0);
                if (shr_sla_list == (void *)-1) {
                        qos_log_critical("Can't map sla shared memory into address space!");
                        return SHR_SLA_MAP_FAIL;
                }
        }
        return 0;
}

int shr_init_sla_sem()
{
        sla_lock = sem_open(SHR_SLA_LOCK, O_CREAT, 0600, 1);
        if (sla_lock == SEM_FAILED) {
                qos_log_critical("SLA semaphore creation failed!");
                return SHR_SLA_SEM_FAIL;
        }
        return 0;
}

void shr_lock_sla()
{
        sem_wait(sla_lock);
}

void shr_unlock_sla()
{
        sem_post(sla_lock);
}

void shr_close_stat()
{
        sem_close(stat_lock);
        shmdt(stat_mem_info);
}

void shr_close_sla()
{
        sem_close(sla_lock);
        shmdt(sla_mem_info);
}