#include "receiver.h"
#include "../lib/logging.c"

/**
	Listen for SLA connections to drop in from the QoS server.
	Receive them, error check, and punt those babies into the kernel.
	That's it.
	Needs to be atomic.
*/

// Used for indicating whether threads should halt.
static int running = 1;

/** BEGIN CLIENT PROGRAM **/
int main(int argc, char *argv[])
{
	printf("Starting QoS receiver.\n");
	setup_clean_kill();
	qos_setup_logging();
	load_client_info();
	receive_slas();
	qos_halt_logging();
	printf("Exiting QoS receiver.\n");
}

int receive_slas() {
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char recvBuff[1025];
	int numrv;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Grabbed a socket.\n");
	memset(&serv_addr, 0, sizeof serv_addr);
	memset(recvBuff, 0, sizeof recvBuff);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(1337);
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof serv_addr);
	if (listen(listenfd, 10) == -1) {
		printf("Failed to listen on socket.\n");
		return 1;
	}
	while (running == 1) {
		connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
		strcpy(recvBuff, "Message from server");
		write(connfd, recvBuff, strlen(recvBuff));
		close(connfd);
	}
	return 0;
}

/**
 * Setup clean kill hook on ctrl+c, etc.
 * On sigaction, perform runhandler() method.
 */
void setup_clean_kill() {
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

void load_client_info()
{

}