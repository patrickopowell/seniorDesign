/**
 * CSC 492 - End-to-End Storage QoS
 * Client User Mode
 * @authors Remington Campbell
 * In charge of sending and receiving data to the client kernel space module as well as to the QoS server located at the storage endpoint.
 * Communicates to the client kernel module by [undetermined, syscall?].
 * Communicates to the QoS server via TCP/IP connection
 */
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sstream>
#include <fstream>

// This should be statically set once determined.
#define SERVERPORT "15555"

// Size of network send/receive buffers.
#define BUFFERLENGTH 5000