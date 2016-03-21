#include "receiver.h"

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
	//load_client_info();
	receive_slas();
	qos_halt_logging();
	printf("Exiting QoS receiver.\n");
}

/**
 * Setup clean kill hook on ctrl+c, etc.
 * On sigaction, perform runhandler(int) method.
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

/**
 * Load client information from configuration file.
 * Fields and usage to be determined.
 */
void load_client_info()
{
	return;
}

/**
 * Receive SLAs 
 */
int receive_slas()
{
	struct sockaddr_in listen_addr;
	char recv_buff[BUFFERLENGTH];
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Grabbed a socket.\n");
	memset(&listen_addr, 0, sizeof listen_addr);
	memset(recvBuff, 0, sizeof recvBuff);
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(LISTENPORT);
	bind(listen_fd, (struct sockaddr*)&listen_addr, sizeof listen_addr);
	if (listen(listen_fd, 10) == -1) {
		printf("Failed to listen on socket.\n");
		return 1;
	}
	while (running == 1) {
		conn_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);
		int n_byte = 0;
		while ((n_byte = read(conn_fd, recv_buff, sizeof(recv_buff) - 1)) > 0) {
			recv_buff[n_byte] = 0;
			if (fputs(recv_buff, stdout) == EOF) {
				printf("\nError Outputting SLA");
			}
			printf("\n");
		}
		if (n_byte < 0)
			printf("Read error.");
	}
	return 0;
}