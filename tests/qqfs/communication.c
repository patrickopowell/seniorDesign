#include "communication.h"

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
                qq_log_critical("Could not create SLA shared memory!");
                return sla_mem_status;
        }

        int sla_sem_status = com_init_sla_sem();
        if (sla_sem_status < 0) {
                qq_log_critical("Could not open SLA semaphore!");
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
                qq_log_debug(strerror(errno));
                return COM_STAT_CREATE_FAIL;
        } else {
                com_stat_list = (struct stat_info_memory *)shmat(shmid_stat, 0, 0);
                if (com_stat_list == (void *)-1) {
                        qq_log_critical("Can't map stat shared memory into address space!");
                        qq_log_debug(strerror(errno));
                        return COM_STAT_MAP_FAIL;
                }
                com_stat_list->count = 0;
        }
        return 0;
}

int com_init_stat_sem()
{
        stat_lock = sem_open(COM_STAT_LOCK, O_CREAT, 0600, 1);
        if (stat_lock == SEM_FAILED) {
                qq_log_critical("Stat semaphore creation failed!");
                qq_log_debug(strerror(errno));
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
                qq_log_debug(strerror(errno));
                return COM_SLA_CREATE_FAIL;
        } else {
                com_sla_list = (struct sla_info_memory *)shmat(shmid_sla, 0, 0);
                if (com_sla_list == (void *)-1) {
                        qq_log_critical("Can't map SLA shared memory into address space!");
                        qq_log_debug(strerror(errno));
                        return COM_SLA_MAP_FAIL;
                }
                com_sla_list->count = 0;
        }
        return 0;
}

int com_init_sla_sem()
{
        sla_lock = sem_open(COM_SLA_LOCK, O_CREAT, 0600, 1);
        if (sla_lock == SEM_FAILED) {
                qq_log_critical("SLA semaphore creation failed!");
                qq_log_debug(strerror(errno));
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

int com_get_sla(char *path, struct sla_info *sla_dest)
{
        int return_val = 0;
        com_lock_sla();
        int found = 0;
        int sla_index = 0;
        int curr_num_slas = com_sla_list->count;
        struct sla_info *curr_sla;
        while (sla_index < curr_num_slas) {
                curr_sla = com_sla_list->slas+sla_index;
                if (strcmp(curr_sla->path, path) == 0) {
                        found = 1;
                        *sla_dest = *curr_sla;
                        break;
                }
                sla_index++;
        }
        if (found == 0)
                return_val = COM_ELEMENT_NFOUND;
        com_unlock_sla();
        return return_val;
}

int com_set_sla(char *path, struct sla_info *sla_src)
{
        int return_val = 0;
        com_lock_sla();
        int found = 0;
        int sla_index = 0;
        int curr_num_slas = com_sla_list->count;
        struct sla_info *curr_sla;
        while (sla_index < curr_num_slas) {
                curr_sla = com_sla_list->slas+sla_index;
                if (strcmp(curr_sla->path, path) == 0) {
                        found = 1;
                        *curr_sla = *sla_src;
                        break;
                }
                sla_index++;
        }
        if (found == 0) {
                qq_log_info("SLA for storage not previously specified, adding to list.");
                if (curr_num_slas == COM_MAX_SERVERS) {
                        qq_log_critical("No more room for additional storage SLAs!");
                        return_val = COM_MEM_OOS;
                } else {
                        com_sla_list->slas[curr_num_slas+1] = *sla_src;
                        com_sla_list->count++;
                }
        }
        com_unlock_sla();
        return return_val;
}

int com_get_stat(char *path, struct stat_info *stat_dest)
{
        int return_val = 0;
        com_lock_stat();
        int found = 0;
        int stat_index = 0;
        int curr_num_stats = com_stat_list->count;
        struct stat_info *curr_stat;
        while (stat_index < curr_num_stats) {
                curr_stat = com_stat_list->stats+stat_index;
                if (strcmp(curr_stat->path, path) == 0) {
                        found = 1;
                        *stat_dest = *curr_stat;
                        break;
                }
                stat_index++;
        }
        if (found == 0)
                return_val = COM_ELEMENT_NFOUND;
        com_unlock_stat();
        return return_val;
}

int com_set_stat(char *path, struct stat_info *stat_src)
{
        int return_val = 0;
        com_lock_stat();
        int found = 0;
        int stat_index = 0;
        int curr_num_stats = com_stat_list->count;
        struct stat_info *curr_stat;
        while (stat_index < curr_num_stats) {
                curr_stat = com_stat_list->stats+stat_index;
                if (strcmp(curr_stat->path, path) == 0) {
                        found = 1;
                        *curr_stat = *stat_src;
                        break;
                }
                stat_index++;
        }
        if (found == 0) {
                qq_log_info("Stats for storage not previously specified, adding to list.");
                if (curr_num_stats == COM_MAX_SERVERS) {
                        qq_log_critical("No more room for additional storage stats!");
                        return_val = COM_MEM_OOS;
                } else {
                        com_stat_list->stats[curr_num_stats+1] = *stat_src;
                        com_stat_list->count++;
                }
        }
        com_unlock_stat();
        return return_val;
}