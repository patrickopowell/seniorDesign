#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef BOOL_H
#define BOOL_H
typedef enum { false, true } bool;
#endif

#ifndef CLIENT_H
#define CLIENT_H

typedef struct Client{
	int sockfd;
	socklen_t address_size;
	struct sockaddr_in address;
	long storage_type;
	long id;
	long current_usage;
	bool above;
	long min;
	long max;
} Client;

Client *createClient(int, socklen_t, struct sockaddr_in, long, long, int, int, long);


//Client createClient(long, long, int, int, struct sockaddr_in *);

#endif
