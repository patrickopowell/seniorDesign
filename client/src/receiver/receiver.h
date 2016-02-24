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

// In case we want to do simulation.
#define SERVERIP "192.168.0.101"
// This should be statically set once determined.
#define SERVERPORT "1337"

// Size of network send/receive buffers.
#define BUFFERLENGTH 1000

 /** FORWARD DECLARATIONS **/
void failure(bool);
void runhandler(int);
void load_client_info();
void *client_to_server_start(void *);
void *server_to_client_start(void *);
int construct_json(char *);
/* void setup_logging();
void log_sensor(unsigned long, float, float);
void log_network(const char *, unsigned long, float, float);
void log_json(char *); */