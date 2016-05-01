#include "main.h"

const char *argp_program_version = "qqclient-0.1";
const char *argp_program_bug_address = "<racampbe@ncsu.edu>";
static char doc[] = "qqclient -- Instantiate QualiQueue on a location.";
static char args_doc[] = "BASE_FOLDER EXPORT_FOLDER QQ_SERVER_IP";
static struct argp_option options[] = {
	{"debug", 'd', 0, 0, "Report debug messages."},
	{0}
};
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[])
{
	struct arguments arguments;
	arguments.debug = 0;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	setup_clean_kill();
	qq_setup_logging("qqclient");
	int lockid = qq_setup_instance();
	int responsibilities = 2;
	pthread_t threads[responsibilities];
	if (pthread_create(&threads[0], NULL, &qq_receiver_start, NULL))
		qq_log_critical("Could not create receiver thread.");
	if (pthread_create(&threads[1], NULL, &qq_parser_start, NULL))
		qq_log_critical("Could not create parser thread.");
	for (int i = 0; i < responsibilities; i++)
		pthread_join(threads[i], NULL);
	qq_halt_logging();
	qq_destroy_instance(lockid);
	qq_log_info("Exiting qqclient.");
	return 0;
}

/**
 * Setup clean kill hook on ctrl+c, etc.
 * On sigaction, perform runhandler(int) method.
 */
void setup_clean_kill()
{
	struct sigaction act;
	act.sa_handler = run_handler;
	sigemptyset(&(act.sa_mask));
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
}

int qq_setup_instance()
{
	qq_log_info("Setting up qqclient instance.");
	com_init_mem();
	int lockid = open(QQCLIENT_LOCK, O_WRONLY | O_CREAT, 0600);
	int rc = flock(lockid, LOCK_EX | LOCK_NB);
	if (rc == -1) {
		qq_log_info("Existing qqclient instance running, inserting server.");
		// add server
		exit(1);
	} else {
		qq_log_info("No qqclient instance detected, starting.");
	}
	running = 1;
	return lockid;
}

void qq_destroy_instance(int lockid)
{
	qq_log_info("Destroying qqclient instance.");
	com_close_mem();
	flock(lockid, LOCK_UN);
}

/**
 * Sets running to 0 to indicate threads should quit.
 * Provides a clean death.
 */
void run_handler(int sig)
{
	running = 0;
}

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;
	switch (key) {
		case 'd':
			arguments->debug = 1;
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num >= 3) // Too many args
				argp_usage(state);
			arguments->args[state->arg_num] = arg;
			break;
		case ARGP_KEY_END:
			if (state->arg_num < 3) // Too few args
				argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}