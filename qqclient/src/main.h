#include <argp.h>
#include "threadcommon/common.h"
#include "receiver/receiver.h"
#include "parser/parser.h"

#define BUFFERLENGTH 5000
#define QQCLIENT_LOCK "/etc/qqclient.lock"

struct arguments {
	char *args[3];
	int debug;
};

int main(int argc, char *argv[]);
void setup_clean_kill();
int qq_setup_instance();
void qq_destroy_instance();
void run_handler(int sig);
static error_t parse_opt(int key, char *arg, struct argp_state *state);