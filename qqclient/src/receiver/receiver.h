/**
 * QualiQueue - Spring 2016
 * @author Remington Campbell <racampbe@ncsu.edu>
 *
 * Receiver functionality for receiving, parsing, and passing of SLAs from QQServer.
 * Places relevant SLA information into shared memory for QQFS's consumption.
 * Communicates with QQServer over TCP.
 */

#include <sys/socket.h>
#include "../threadcommon/common.h"
#include "../../../libcommon/qqjson/qqjson.h"

#define LISTENPORT "15555"

// Size of network send/receive buffers.
#define BUFFERLENGTH 5000

// Number of potential SLA connections to queue
#define SLA_BACKLOG 5

/** FORWARD DECLARATIONS **/
void *qq_receiver_start(void *in);