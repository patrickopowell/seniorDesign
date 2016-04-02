#include "main.h"

static int running = 1;

static int critical_send_feedback = 0;

int main(int argc, char *argv[])
{
	setup_clean_kill();
	pthread_t threads[RESPONSIBILITIES];
	if (pthread_create(&threads[0], NULL, &receiver_start, NULL))
		perror("Could not create receiver thread!\n");
	if (pthread_create(&threads[1], NULL, &parser_start, NULL))
		perror("Could not create parser thread!\n");
	for (int i = 0; i < RESPONSIBILITIES; i++)
		pthread_join(threads[i], NULL);
	printf("Exiting main thread.\n");
}

/**
 * Setup clean kill hook on ctrl+c, etc.
 * On sigaction, perform runhandler(int) method.
 */
void setup_clean_kill()
{
	struct sigaction act;
	act.sa_handler = runhandler;
	sigemptyset(&(act.sa_mask));
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
}

/**
 * Sets running to 0 to indicate threads should quit.
 * Provides a clean death.
 */
void runhandler(int sig)
{
	running = 0;
}

void failure(int is_thread)
{
	if (is_thread > 0)
		pthread_exit(NULL);
	else
		exit(1);
}