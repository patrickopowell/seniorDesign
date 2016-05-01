#include <argp.h>
#include "common.h"
#include "receiver/receiver.h"
#include "parser/parser.h"

#define RESPONSIBILITIES 2
#define BUFFERLENGTH 5000
#define QQCLIENT_LOCK "/etc/qqclient.lock"

struct arguments {
	char *args[3];
	int debug;
};

int main(int argc, char *argv[]);
void setup_clean_kill();
void setup_instance();
void run_handler(int sig);
/* check_running() moved to common.h */
static error_t parse_opt(int key, char *arg, struct argp_state *state);