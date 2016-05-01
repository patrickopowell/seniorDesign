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
	int lockid = qos_setup_instance();
	qos_setup_logging("qqclient");
	pthread_t threads[RESPONSIBILITIES];
	if (pthread_create(&threads[0], NULL, &qos_receiver_start, NULL))
		qos_log_critical("Could not create receiver thread.");
	if (pthread_create(&threads[1], NULL, &qos_parser_start, NULL))
		qos_log_critical("Could not create parser thread.");
	for (int i = 0; i < RESPONSIBILITIES; i++)
		pthread_join(threads[i], NULL);
	qos_log_info("Exiting qqclient.");
	qos_halt_logging();
	qos_destroy_instance(lockid);
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

void qos_setup_instance()
{
	shr_init_mem();
	int lockid = open(QQCLIENT_LOCK, O_WRONLY | O_CREAT, 0600);
	int rc = flock(lockfile, LOCK_EX | LOCK_NB);
	if (rc == -1) {
		qos_log_info("Existing qqclient instance running, inserting server.");
		// add server
		exit(1);
	} else {
		
	}
	running = 1;
	return lockid;
}

void qos_destroy_instance(int lockid)
{
	shr_close_mem();
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