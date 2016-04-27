#include "common.h"
#include "receiver/receiver.h"
#include "parser/parser.h"

#define RESPONSIBILITIES 2
#define BUFFERLENGTH 5000

int main(int argc, char *argv[]);
void setup_clean_kill();
void run_handler(int sig);
int check_running();
void failure(int is_thread);