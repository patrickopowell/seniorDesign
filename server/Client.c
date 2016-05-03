#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef enum { false, true } bool;

/* Function typedefs for setters */
typedef void (*f1)(long);
typedef void (*f2)(bool);

typedef struct Client{
	/*f1 F1; // set id
	f1 F2; // set current_usage
	f1 F3; // set min
	f1 F4; // set max
	f2 F5; // set above*/
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
//Client *createClient(long ID, long curr, int MIN, int MAX, struct sockaddr_in *incoming_addr){
Client *createClient(long ID, long curr, int MIN, int MAX, struct sockaddr_in address, long storage_type){
	Client *c1 = (Client *)malloc(sizeof(Client));

	//c1->storage_type = malloc(50*sizeof(char));
	//strcpy(c1->storage_type, storage_type);
	c1->storage_type = storage_type;
	c1->address = address;
	c1->id = ID;
	c1->current_usage = curr;
	c1->min = MIN;
	c1->max = MAX;
	c1->above = false;
	return c1;
}
