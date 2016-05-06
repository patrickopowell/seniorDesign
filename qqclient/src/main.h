#include <argp.h>
#include "threadcommon/common.h"
#include "receiver/receiver.h"
#include "parser/parser.h"

// Size of buffer for sending and receiving SLAs
#define BUFFERLENGTH 5000

// System permission issues, cannot run QQFS as sudo to access /etc/.
//#define QQCLIENT_LOCK "/etc/qqclient.lock"
#define QQCLIENT_LOCK "qqclient.lock"

// Arguments structure for args, using argp.
struct arguments {
	char *args[3];
};

int main(int argc, char *argv[]);
void setup_clean_kill();
int qq_setup_instance();
void qq_destroy_instance();
void qq_unmount_instances();
void run_handler(int sig);
static error_t parse_opt(int key, char *arg, struct argp_state *state);