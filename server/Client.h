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

typedef struct {
	long storage_type;
	struct sockaddr_in address;
	long id;
	long current_usage;
	bool above;
	int min;
	int max;
} Client;

Client *createClient(long, long, int, int, struct sockaddr_in, long);


//Client createClient(long, long, int, int, struct sockaddr_in *);

#endif
