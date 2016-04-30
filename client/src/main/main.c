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
	setup_instance();
	qos_setup_logging();
	pthread_t threads[RESPONSIBILITIES];
	if (pthread_create(&threads[0], NULL, &qos_receiver_start, NULL))
		qos_log_critical("Could not create receiver thread.");
	if (pthread_create(&threads[1], NULL, &qos_parser_start, NULL))
		qos_log_critical("Could not create parser thread.");
	for (int i = 0; i < RESPONSIBILITIES; i++)
		pthread_join(threads[i], NULL);
	qos_log_info("Exiting qqclient.");
	qos_halt_logging();
	flock(QQCLIENT_LOCK, LOCK_UN);
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

void setup_instance()
{
	int fd = open(QQCLIENT_LOCK, OWRONLY | O_CREAT, 0600);
	int rc = flock(fd, LOCK_EX | LOCK_NB);
	if (rc == -1) {
		qos_log_critical("Existing qqclient instance running.");
		// add server
		exit(1);
	}
	running = 1;
}

/**
 * Sets running to 0 to indicate threads should quit.
 * Provides a clean death.
 */
void run_handler(int sig)
{
	running = 0;
}

int check_running()
{
	return running;
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