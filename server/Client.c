#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef enum { false, true } bool;

/* Function typedefs for setters */
typedef void (*f1)(long);
typedef void (*f2)(bool);

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

/**
 * Caller must free whatever this function returns.
 */
Client *createClient(int sockfd, socklen_t address_size, struct sockaddr_in address, long ID, long curr, int MIN, int MAX, long storage_type){
	Client *c1 = (Client *)malloc(sizeof(Client));
	c1->sockfd = sockfd;
	c1->address_size = address_size;
	c1->address = address;
	c1->storage_type = storage_type;
	c1->id = ID;
	c1->current_usage = curr;
	c1->min = MIN;
	c1->max = MAX;
	c1->above = false;
	return c1;
}
