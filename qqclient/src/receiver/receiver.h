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

#define LISTENPORT "15555"

// Size of network send/receive buffers.
#define BUFFERLENGTH 5000

// Number of potential SLA connections to queue
#define SLA_BACKLOG 5

/** FORWARD DECLARATIONS **/
void *qq_receiver_start(void *in);