/** COMMON INCLUDES **/
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>

/** INCLUDE MODULES **/
#include "receiver/receiver.h"
#include "parser/parser.h"
#include "../lib/qos-json/qos-json.h"

#define RESPONSIBILITIES 2
#define BUFFERLENGTH 5000
#define SERVERPORT "15555"