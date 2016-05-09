/**
 * QualiQueue - Spring 2016
 * @author Remington Campbell <racampbe@ncsu.edu>
 *
 * Entry point of QQClient program.
 * QQClient is responsible for starting and managing QQFS instances
 * as part of the QualiQueue system - configured by a QQServer.
 */

#include "main.h"

/** ARGP SPECIFICATION **/
const char *argp_program_version = "qqclient-0.1";
const char *argp_program_bug_address = "<racampbe@ncsu.edu>";
static char doc[] = "qqclient -- Instantiate QualiQueue on a location.";
static char args_doc[] = "BASE_FOLDER EXPORT_FOLDER QQSERVER_IP QQSTORAGE_ID";
static struct argp_option options[] = {
	{0}
};
static struct argp argp = {options, parse_opt, args_doc, doc};

/**
 * Main entry point for QQClient.
 * Sets up OS signal handling and handles instance initialiation and cleanup.
 *
 * @author Remington Campbell <racampbe@ncsu.edu>
 */
int main(int argc, char *argv[])
{
	struct arguments arguments;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	setup_clean_kill();
	int lockid = qq_setup_instance(argv[1], argv[2], argv[3], argv[4]);
	qq_destroy_instance(lockid);
	return 0;
}

/**
 * Setup clean kill hook on ctrl+c, etc.
 * On sigaction, perform runhandler(int) method.
 *
 * @author Remington Campbell <racampbe@ncsu.edu>
 */
void setup_clean_kill()
{
	struct sigaction act;
	act.sa_handler = run_handler;
	sigemptyset(&(act.sa_mask));
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTERM, &act, 0);
	sigaction(SIGHUP, &act, 0);
	sigaction(SIGQUIT, &act, 0);
}

/**
 * Set up the single instance QualiQueue application.
 * If an instance is already running, start/insert QQFS instance into the existing, managed
 * shared memory of QQClient, then exit.
 * Otherwise, initalize QQClient-specific shared memory and start the QQFS instance.
 * Starting QQFS is accomplished via system calls.
 * 
 * @author Remington Campbell <racampbe@ncsu.edu>
 */
int qq_setup_instance(char *base_path, char *export_path, char *qqserver_ip, char *qqstorage_id)
{
	qq_setup_logging("qqclient");
	qq_log_info("Setting up qqclient instance.");
	running = 1;
	int lockid = open(QQCLIENT_LOCK, O_WRONLY | O_CREAT, 0600);
	if (lockid < 0) {
		qq_log_critical("Could not open lock file! Fatal error.");
		qq_log_debug(strerror(errno));
		exit(1);
	}
	int rc = flock(lockid, LOCK_EX | LOCK_NB);
	int mem_status = qq_init_mem();
	if (mem_status < 0) {
		qq_log_critical("Could not map qqclient shared memory! Fatal error.");
		exit(1);
	}
	char qqfs_command[1024];
	snprintf(qqfs_command, sizeof qqfs_command, "qqfs %s %s", base_path, export_path);
	if (rc == -1) {
		qq_log_info("Existing qqclient instance running, inserting server.");
		struct qqfs_instance *new_instance = calloc(1, sizeof(struct qqfs_instance));
		memcpy(new_instance->base_path, base_path, strlen(base_path));
		memcpy(new_instance->export_path, export_path, strlen(export_path));
		memcpy(new_instance->qqserver_ip, qqserver_ip, strlen(qqserver_ip));
		new_instance->qqstorage_id = strtol(qqstorage_id, NULL, 10);
		qq_set_qqfs_instance(new_instance);
		free(new_instance);
		if (system(qqfs_command) != 0)
			qq_log_critical("QQFS was not successfully started!");
		qq_close_mem();
		exit(0);
	} else {
		qq_log_info("No qqclient instance detected, starting.");
		com_init_mem();
		struct qqfs_instance *new_instance = calloc(1, sizeof(struct qqfs_instance));
		memcpy(new_instance->base_path, base_path, strlen(base_path));
		memcpy(new_instance->export_path, export_path, strlen(export_path));
		memcpy(new_instance->qqserver_ip, qqserver_ip, strlen(qqserver_ip));
		new_instance->qqstorage_id = strtol(qqstorage_id, NULL, 10);
		qq_set_qqfs_instance(new_instance);
		free(new_instance);
		if (system(qqfs_command) != 0) {
			qq_log_critical("QQFS was not successfully started! This is required for instance creation.");
		} else {
			int responsibilities = 2;
			pthread_t threads[responsibilities];
			if (pthread_create(&threads[0], NULL, &qq_receiver_start, NULL))
				qq_log_critical("Could not create Receiver.");
			if (pthread_create(&threads[1], NULL, &qq_parser_start, NULL))
				qq_log_critical("Could not create Parser.");
			for (int i = 0; i < responsibilities; i++)
				pthread_join(threads[i], NULL);
		}
	}
	return lockid;
}

/**
 * Cleanup QQClient managed system resources.
 * Unmount QQFS instances, close all shared memory, and unlock lockfile.
 *
 * @author Remington Campbell <racampbe@ncsu.edu>
 */
void qq_destroy_instance(int lockid)
{
	qq_log_info("Destroying qqclient instance.");
	qq_unmount_instances();
	com_close_mem();
	qq_close_mem();
	qq_halt_logging();
	flock(lockid, LOCK_UN);
}

/**
 * Unmount all managed QQFS instances.
 * Accomplished via system calls.
 *
 * @author Remington Campbell <racampbe@ncsu.edu>
 */
void qq_unmount_instances()
{
	qq_log_info("Detaching QQFS instances from destinations.");
	qq_lock();
	for (int i = 0; i < qqfs_instance_list->count; i++) {
		struct qqfs_instance *curr_inst = &(qqfs_instance_list->instances[i]);
		char qqfs_ucommand[1024];
		snprintf(qqfs_ucommand, sizeof qqfs_ucommand, "fusermount -u %s", curr_inst->export_path);
		if (system(qqfs_ucommand) != 0)
			qq_log_error("Unable to detatch QQFS from destination!");
	}
	qq_unlock();
}

/**
 * Sets running to 0 to indicate threads should quit.
 * Provides a clean death indicator to all threads.
 * Can also be used to intercept and trigger events based on signal received.
 *
 * @author Remington Campbell <racampbe@ncsu.edu>
 */
void run_handler(int sig)
{
	running = 0;
}

/**
 * Parse arguments/options passed to QQClient.
 * Handled by argp.
 *
 * @author Remington Campbell <racampbe@ncsu.edu>
 */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;
	switch (key) {
		case ARGP_KEY_ARG:
			if (state->arg_num >= 4) // Too many args
				argp_usage(state);
			arguments->args[state->arg_num] = arg;
			break;
		case ARGP_KEY_END:
			if (state->arg_num < 4) // Too few args
				argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}