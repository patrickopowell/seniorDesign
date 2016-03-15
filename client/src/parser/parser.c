#include "parser.h"

// Used for indicating whether threads should halt.
static bool running = 1;

// Locks access to the readings between threads.
static pthread_mutex_t data_lock = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
	struct timeval loadtime;
	string node_id;
} client;

FILE *kernel_log;
FILE *network_log;

/** FORWARD DECLARATIONS **/
void failure(bool);
void runhandler(int);
void load_client_info();
void *client_to_server_start(void *);
void *server_to_client_start(void *);
int construct_json(char *);

/** BEGIN CLIENT PROGRAM **/
int main(int argc, char *argv[])
{
	setup_clean_kill();
	qos_setup_logging();
	load_client_info();
	pthread_t threads[RESPONSIBILITIES];
	if (pthread_create(&threads[0], NULL, &server_to_client_start, NULL))
		perror("Could not create SLA thread.\n");
	if (pthread_create(&threads[1], NULL, &client_to_server_start, NULL))
		perror("Could not create server feedback thread.\n");
	for (int i = 0; i < RESPONSIBILITIES; i++)
		pthread_join(threads[i], NULL);
	fclose(kernel_log);
	fclose(network_log);
	printf("Exiting main thread.\n");
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

void failure(bool isThread) {
	if (isThread)
		pthread_exit(NULL);
	else
		exit(0);
}

/**
 * Sets running to 0 to indicate threads should quit.
 * Provides a clean death.
 */
void runhandler(int sig)
{
	running = false;
}

void load_client_info()
{
	return;
}

/**
 * Thread entry method for client SLA responsibilities.
 */
void *server_to_client_start(void *in)
{
	printf("Client SLA thread started.\n");
	while (running) {
		/**
		 * Listen for server SLA.
		 * Hand off SLA to kernel.
		 */
		 usleep(2250000); // Sleep well, sweet prince.
	}
	printf("Exiting Client SLA thread.\n");
	pthread_exit(NULL);
}

/**
 * Thread entry method for client feedback responsibilities.
 */
void *client_to_server_start(void *in)
{
	printf("Client feedback thread started.\n");
	// Copying and pasting some network code I've used in the past.
	char recvbuffer[BUFFERLENGTH];
	char sendbuffer[BUFFERLENGTH];
	int sockfd;
	int status;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	if ((status = getaddrinfo(SERVERIP, SERVERPORT, &hints, &res)) != 0) {
		perror(gai_strerror(status));
		failure(true);
	}

	if (res == NULL) {
		perror("No address info available.\n");
		failure(true);
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0) {
		perror("Could not create a socket!\n");
		failure(true);
	}

	char ip[INET_ADDRSTRLEN];
	inet_ntop(res->ai_family, &(((struct sockaddr_in *)res->ai_addr)->sin_addr), ip, sizeof ip);
	printf("Socket created to %s:%s.\n", ip, SERVERPORT);

	if (connect(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
		perror("Issue with socket binding.\n");
		failure(true);
	}

	struct sockaddr_storage server_addr;
	socklen_t server_len = sizeof server_addr;

	struct timeval stTimeOut;
	stTimeOut.tv_sec = 2;
	stTimeOut.tv_usec = 250000;

	fd_set master, read_fd;
	FD_ZERO(&master);
	FD_SET(sockfd, &master);

	int sensor_error = 0;
	int server_timeout = 0;

	while (running) {
		// More code, do not run this as is. Will spam packets if somehow compiles.
		int msg_length = construct_json(sendbuffer);
		log_json(sendbuffer);
		int numbytes = sendto(sockfd, sendbuffer, msg_length, 0, res->ai_addr, res->ai_addrlen);
		if (numbytes == -1) {
			// Don't remember what this indicates, need to look it up.
			// log_network("node", 0, 0, 0);
			usleep(2250000);
			continue;
		} else if (numbytes == msg_length)
			// log_network("node", time(NULL), _readings[0], _readings[1]);
		read_fd = master;
		if (select(sockfd+1, &read_fd, NULL, NULL, &stTimeOut) == -1) {
			perror("Could not obtain select for socket.\n");
			usleep(2250000);
			continue;
		}
		if (!FD_ISSET(sockfd, &read_fd)) {
			stTimeOut.tv_sec = 2;
			stTimeOut.tv_usec = 250000;
			server_timeout++;
			if (server_timeout > 4)
				// do something, server hasn't responded for 4 cycles
			log_network("server", 0, 0, 0);
			perror("No response received from server.\n");
		} else {
			server_timeout = 0;
			numbytes = recvfrom(sockfd, recvbuffer, BUFFERLENGTH-1, 0, (struct sockaddr *)&server_addr, &server_len);
			if (numbytes == -1) {
				perror("Could not setup receive from server.\n");
				// Do something
			} else if (numbytes == 0) {
				printf("Server disconnected.\n");
				// Do something
			} else {
				// Assuming we are using JSON at any point.
				// Legacy code, might be useful.
				rapidjson::Document d;
				d.Parse(recvbuffer);
				//string userID = d["userID"].GetString();
				memset(&recvbuffer, 0, numbytes);
			}
			usleep(2250000);
		}
	}

	close(sockfd);
	freeaddrinfo(res);

	printf("Exiting client feedback thread.\n");
	pthread_exit(NULL);
}

int construct_json(char *buffer) {
	/* float _readings[2];
	read_sensor(_readings);
	string msg = "{\"userID\":\"" + node_id + "\",\"temperature\":" + SSTR(_readings[0]) + ",\"humidity\":" + SSTR(_readings[1]) + "}";
	int length = msg.size();
	memcpy(buffer, msg.c_str(), length + 1);
	return length; */
}

/* void write_sensor(float temperature, float humidity)
{
	pthread_mutex_lock(&sensor_lock);
		if (temperature > 0 && humidity > 0) {
			last_temperature = readings.temperature;
			last_humidity = readings.humidity;
		}
		gettimeofday(&(readings.timeof), NULL);
		readings.temperature = temperature;
		readings.humidity = humidity;
		log_sensor(readings.timeof.tv_sec, readings.temperature, readings.humidity);
	pthread_mutex_unlock(&sensor_lock);
}

void read_sensor(float *_readings)
{
	pthread_mutex_lock(&sensor_lock);
		_readings[0] = readings.temperature;
		_readings[1] = readings.humidity;
	pthread_mutex_unlock(&sensor_lock);
} */

/** LOGGING METHODS
void setup_logging() {
	sensor_log = fopen("sensor_log.txt", "w");
	network_log = fopen("network_log.txt", "w");
	json_log = fopen("json_log.txt", "w");
	fprintf(sensor_log, "%15s%7s%7s\n", "Timestamp", "Temperature", "Humidity");
	fprintf(network_log, "%12s%15s%7s%7s\n", "Origin", "Timestamp", "Temperature", "Humidity");
	fprintf(json_log, "%s\n", "JSON I/O");
}

void log_sensor(unsigned long timestamp, float temperature, float humidity) {
	printf("%15ul%7.2f%7.2f\n", timestamp, temperature, humidity);
	fprintf(sensor_log, "%15ul%7.2f%7.2f\n", timestamp, temperature, humidity);
}

void log_network(const char *origin, unsigned long timestamp, float temperature, float humidity) {
	printf("%12s%15ul%7.2f%7.2f\n", origin, timestamp, temperature, humidity);
	fprintf(network_log, "%12s%15ul%7.2f%7.2f\n", origin, timestamp, temperature, humidity);
}

void log_json(char *json) {
	fprintf(json_log, "%s\n", json);
} */