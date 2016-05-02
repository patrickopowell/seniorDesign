#include "common.h"

static key_t mem_key;
static sem_t *mem_lock;

int check_running()
{
	return running;
}

int qq_init_mem()
{
	int qq_mem_status = qq_init_qqmap_mem();
	if (qq_mem_status < 0) {
		qq_log_critical("Could not create qqclient shared memory!");
		return qq_mem_status;
	}

	int qq_sem_status = qq_init_qqmap_sem();
	if (qq_sem_status < 0) {
		qq_log_critical("Could not open qqclient semaphore!");
		return qq_sem_status;
	}
}

int qq_init_qqmap_mem()
{
	mem_key = ftok(QQCLIENT_MEM, 1);
	int shmid_mem = shmget(mem_key, sizeof(struct qqfs_instances), 0600 | IPC_CREAT);
	if (shmid_mem == -1) {
		qq_log_critical("Can't create qqclient shared memory!");
		qq_log_debug(strerror(errno));
		return QQCLIENT_MEM_CREATE_FAIL;
	} else {
		qqfs_instance_list = (struct qqfs_instances *)shmat(shmid_mem, 0, 0);
		if (qqfs_instance_list == (void *)-1) {
			qq_log_critical("Can't map qqclient shared memory into address space!");
			qq_log_debug(strerror(errno));
			return QQCLIENT_MEM_MAP_FAIL;
		}
		qqfs_instance_list->count = 0;
	}
	return 0;
}

int qq_init_qqmap_sem()
{
	mem_lock = sem_open(QQCLIENT_SEM, O_CREAT, 0600, 1);
	if (mem_lock == SEM_FAILED) {
		qq_log_critical("qqclient semaphore creation failed!");
		qq_log_debug(strerror(errno));
		return QQCLIENT_SEM_FAIL;
	}
	return 0;
}

void qq_close_mem()
{
	sem_close(mem_lock);
	shmdt(qqfs_instance_list);
}

void qq_lock()
{
	sem_wait(mem_lock);
}

void qq_unlock()
{
	sem_post(mem_lock);
}

int qq_get_qqfs_instance(char *export_path, struct qqfs_instance *instance_dest)
{
	int return_val = 0;
	qq_lock();
	int found = 0;
	int qqfs_index = 0;
	int curr_num_qqfs = qqfs_instance_list->count;
	struct qqfs_instance *curr_qqfs;
	while (qqfs_index < curr_num_qqfs) {
		curr_qqfs = &(qqfs_instance_list->instances[qqfs_index]);
		if (strcmp(curr_qqfs->export_path, export_path) == 0) {
			found = 1;
			memcpy(&curr_qqfs, &instance_dest, sizeof curr_qqfs);
			break;
		}
		qqfs_index++;
	}
	if (found == 0)
		return_val = QQCLIENT_ELEMENT_NFOUND;
	qq_unlock();
	return return_val;
}

int qq_set_qqfs_instance(struct qqfs_instance *instance_src)
{
	int return_val = 0;
	qq_lock();
	int found = 0;
	int qqfs_index = 0;
	int curr_num_qqfs = qqfs_instance_list->count;
	struct qqfs_instance *curr_qqfs;
	while (qqfs_index < curr_num_qqfs) {
		curr_qqfs = &(qqfs_instance_list->instances[qqfs_index]);
		if (strcmp(curr_qqfs->export_path, instance_src->export_path) == 0) {
			found = 1;
			qq_log_critical("Cannot duplicate QQFS mount exports!");
			return_val = QQCLIENT_DUP_EXPORT;
			break;
		}
		qqfs_index++;
	}
	if (found == 0) {
		if (curr_num_qqfs == QQCLIENT_MAX_QQFS) {
			qq_log_critical("No more room for additional QQFS instances!");
			return_val = QQCLIENT_MEM_OOS;
		} else {
			curr_qqfs = &(qqfs_instance_list->instances[curr_num_qqfs]);
			memcpy(&instance_src, &curr_qqfs, sizeof instance_src);
			qqfs_instance_list->count++;
		}
	}
	qq_unlock();
	return return_val;
}

int qq_get_num_qqfs_instances()
{
	int num_instances = 0;
	qq_lock();
	num_instances = qqfs_instance_list->count;
	qq_unlock();
	return num_instances;
}

int qq_get_qqfs_instance_by_idx(int index, struct qqfs_instance *instance_dest)
{
	int return_val = 0;
	qq_lock();
	if (index > qqfs_instance_list->count-1)
		return_val = QQCLIENT_ELEMENT_NFOUND;
	else {
		struct qqfs_instance *curr_qqfs;
		curr_qqfs = &(qqfs_instance_list->instances[index]);
		memcpy(&curr_qqfs, &instance_dest, sizeof curr_qqfs);
	}
	qq_unlock();
	return return_val;
}
