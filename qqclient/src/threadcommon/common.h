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
#include <netdb.h>
#include <string.h>
#include "../../libcommon/logging/logging.h"
#include "../../libcommon/client/communication.h"

typedef struct storage_server storage_server;

struct storage_server {
	int s_dev;
	int i_ino;
	struct addrinfo hints;
	struct addrinfo *receiver_info;
	storage_server *next;
};

storage_server *server_list;

int running;

int check_running();
#define COMMON_INCLUDE
#endif