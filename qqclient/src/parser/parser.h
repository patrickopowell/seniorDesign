/**
 * CSC 492 - End-to-End Storage qq
 * Client User Mode
 * @authors Remington Campbell
 * In charge of sending and receiving data to the client kernel space module as well as to the qq server located at the storage endpoint.
 * Communicates to the client kernel module by [undetermined, syscall?].
 * Communicates to the qq server via TCP/IP connection
 */

#include "../common.h"
#include "../../lib/qq-json/qq-json.h"

// This should be statically set once determined.
#define SERVERPORT "15555"
#define DEBUG_SERVER "192.168.1.5"

// Size of network send/receive buffers.
#define BUFFERLENGTH 5000
 
void *qq_parser_start(void *in);
void *qq_feedback_timer(void *in);