#ifndef COMMON_INCLUDE
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include "../../../libcommon/protocol.h"
#include "../../../libcommon/logging/logging.h"
#include "../../../libcommon/client/communication.h"

#define QQCLIENT_MEM "qq-client-mem"
#define QQCLIENT_SEM "qq-client-lock"

#define QQCLIENT_MEM_CREATE_FAIL -1
#define QQCLIENT_MEM_MAP_FAIL -2
#define QQCLIENT_SEM_FAIL -3
#define QQCLIENT_ELEMENT_NFOUND -4
#define QQCLIENT_MEM_OOS -5
#define QQCLIENT_DUP_EXPORT -6

#define QQCLIENT_MAX_QQFS 5

struct qqfs_instance {
	char qqserver_ip[15];
	char base_path[40];
	char export_path[40];
	int qqstorage_id;
	int qqserver_socket;
	int sla_version;
	int storage_type;
};

struct qqfs_instances {
	struct qqfs_instance instances[QQCLIENT_MAX_QQFS];
	int count;
};

struct qqfs_instances *qqfs_instance_list;

int running;

int check_running();
int qq_init_mem();
int qq_init_qqmap_mem();
int qq_init_qqmap_sem();
void qq_close_mem();
void qq_lock();
void qq_unlock();
int qq_get_qqfs_instance(char *export_path, struct qqfs_instance *instance_dest);
int qq_set_qqfs_instance(struct qqfs_instance *instance_src);
int qq_update_qqfs_instance(struct qqfs_instance *instance_src);
int qq_get_num_qqfs_instances();
int qq_get_qqfs_instance_by_idx(int index, struct qqfs_instance *instance_dest);
int qq_get_qqfs_instance_by_pair(char *qqserver_ip, int qqstorage_id, struct qqfs_instance *instance_dest);
#define COMMON_INCLUDE
#endif