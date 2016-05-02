/**
 * CSC 492 - End-to-End Storage qq
 * Client User Mode
 * @authors Remington Campbell
 * In charge of sending and receiving data to the client kernel space module as well as to the qq server located at the storage endpoint.
 * Communicates to the client kernel module by [undetermined, syscall?].
 * Communicates to the qq server via TCP/IP connection
 */

#include "../threadcommon/common.h"
#include "../../../libcommon/qqjson/qqjson.h"

// This should be statically set once determined.
#define SERVERPORT "15555"

// Size of network send/receive buffers.
#define BUFFERLENGTH 5000
 
void *qq_parser_start(void *in);
void qq_parse_stat(struct stat_info *stat);
int qq_init_connection(struct qqfs_instance *instance);
void qq_send_handshake(struct qqfs_instance *instance);
void qq_send_stat(struct qqfs_instance *instance, struct stat_info *stat);
void qq_close_connections();