/******************************************************************
 * This is the interface that will communicate with the client.
 *
 * As of Tuesday May 3, 2016, to compile:
 * 		# gcc -Wall -std=c99 -g interface.c Client.c data_structures.c monitor.c Parser.c -o interface.out
 *
 *****************************************************************/



#include "data_structures.h"
#include "monitor.h"
#include "Client.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_ONE 15555 // The server port clients will initially connect to.
#define FOREIGN_PORT 15555 // The client port we are sending SLA info to.

/* Global variables. */
struct sockaddr_in local_addr; // my address
struct sockaddr_in client_addr; // client address
char client_string[1024]; // stores stuff the client sends me with connection request... ID, IOPS, etc.
char SLA[1024]; // sends to Client 
socklen_t sin_size = NULL;
int sock; // Socket for listening.
int new_sock; // Socket for connect()/accept()
int send_SLA_sock;
int protocol_version = 0;
//int SLA_version = 0;

int sockOption = NULL;
int binding = NULL;
int bl_size = NULL;
int listening = NULL;
	
int SLA_unused = NULL;

LinkedList *list; // = createList();
Parser *parser; // = createParser();
Client *client;
Node *head; // = (Node *)malloc(sizeof(Node));


/**
 * Setup the sockets and stuff.
 */
void setup()
{
	list = createList();
	parser = createParser();
	head = (Node *)malloc(sizeof(Node));

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT_ONE);
    local_addr.sin_addr.s_addr = 0;
    memset(&(local_addr.sin_zero), '\0', 8); // Zero rest of struct
    ////////////////////////////////////////////////////////////////
    int domain = PF_INET;
    int type = SOCK_STREAM;
    int protocol = 0;
    sock = socket(domain, type, protocol); // create a socket
	if (sock < 0){
	    printf("Error opening socket! \n");
	}
	////////////////////////////////////////////////////////////////
	// Allows us to use this socket for binding even if it seems to be in use, already.
	int yes = 1;
	sockOption = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	if(sockOption == -1){
        printf("Error setting socket option to SO_REUSEADDR \n");
    }
	////////////////////////////////////////////////////////////////
	socklen_t addr_length = sizeof(struct sockaddr);
	binding = bind(sock, (struct sockaddr *) &local_addr, addr_length);
	if(binding != 0){
	    //error
	}
	////////////////////////////////////////////////////////////////
	bl_size = 5; // backlog size
	listening = listen(sock, bl_size);
	if(listening != 0){
	    //error
	}
}

/*****************************************************************************************************
 * Accepts a connection. Reads information from the client for SLA negotiation.
 * 
 ****************************************************************************************************/
int getClient()
{
	// Start accepting connections
    sin_size = sizeof(struct sockaddr_in);
    new_sock = accept(sock, (struct sockaddr *) &client_addr, &sin_size); // returns new socket file-descriptor
    if(new_sock < 0){
        return 0; // failure
    } else{
    	incrementNumClients();
    	increment_SLA();
        recv(new_sock, &client_string, 1024, 0);
        ///// Parse the received string. ///////////
        long id = parser->F1(client_string);
        long curr_usage = NULL; // = parser->F10(client_string); //TODO
        long min = parser->F8(client_string);
        long max = parser->F7(client_string);
        long storage_type = parser->F4(client_string);
        //// Use the data to create a Client. /////
        client = createClient(id, curr_usage, min, max, client_addr, storage_type);
        list->F11(&head, *client); // push client into front of list
        printf("Server: got connection from %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Pushing new SLA to all clients... \n");
        return 1; // success
    }
}

/*******************************************************************************************************
 * Making the JSON string is too nasty to appear in my main function. So I'm making
 * this helper function to interact with the monitor and create the SLA JSON string.
 * The parameters are things that the server either gets from the user or keeps track 
 * of in the main function. Everything else in the SLA is determined by the monitor.
 * @param protocol_version - Determined in main.
 * @param client_id        - Received from the client.
 * @param storage_type     - Received from the client.
 ******************************************************************************************************/
void makeSLA( int protocol_version, long client_id, long storage_type){
	long SLA_version = getSLA_version();
    int iops_max = getMaxIOPS();
    int iops_min = getMinIOPS();
    int thru_max = getMaxThru();
    int thru_min = getMinThru();
    int SLA_unused = getUnused();
    //char SLA[1024];
	snprintf(SLA, sizeof(SLA), \
	    "{\"protocol_version_number\":%d, \
	        \"version\":%lu, \
	        \"client_id\":%lu, \
	        \"storage_type\":%lu, \
	        \"iops_max\":%d, \
	        \"iops_min\":%d, \
	        \"throughput_max\":%d, \
	        \"throughput_min\":%d, \
	        \"SLA_unused\":%d}",
	    protocol_version, SLA_version, client_id, storage_type, iops_max, iops_min, thru_max, thru_min, SLA_unused);
    //return SLA;
}

void pushSLA()
{
	//int len = list->F14(head);
	long cli_ID = NULL;
	long storage_type = NULL;
	Node* current = head;
	while (current != NULL) {
		cli_ID = (current->c).id;
		storage_type = (current->c).storage_type;
		makeSLA(1, cli_ID, storage_type);
		send(new_sock, SLA, sizeof(SLA), 0); // just a random string for testing.
		current = current->next;
	}
}

int main(void)
{
    setup();
    //makeSLA(3, 3, "Hello"); // for testing purposes
    //printf("%s\n", SLA); // Also for the test
    while(1){
		if(getClient()){
			pushSLA();
		}
    }
    return 0;
}























