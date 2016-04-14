/**
 * CSC 492 - End-to-End Storage QoS
 * Client User Mode
 * @authors Remington Campbell
 * In charge of sending and receiving data to the client kernel space module as well as to the QoS server located at the storage endpoint.
 * Communicates to the client kernel module by [undetermined, syscall?].
 * Communicates to the QoS server via TCP/IP connection
 */

#include "../common.h"
#include "../../lib/qos-json/qos-json.h"

// This should be statically set once determined.
#define LISTENPORT 15555

// Size of network send/receive buffers.
#define BUFFERLENGTH 5000

// Number of potential SLA connections to queue
#define SLA_BACKLOG 5

/** FORWARD DECLARATIONS **/
void *qos_receiver_start(void *in);