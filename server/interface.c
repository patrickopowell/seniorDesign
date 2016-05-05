/******************************************************************
 * This is the interface that will communicate with the client.
 *
 * As of Tuesday May 3, 2016, to compile:
 * 		# gcc -Wall -std=c99 -g interface.c Client.c data_structures.c monitor.c Parser.c -o interface.out
 * To check for memory leaks:
 * 		# valgrind --leak-check=full --show-leak-kinds=all ./interface.out
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
#include <unistd.h> // for closing the sockets...

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

/* Global variables. They are initialized in the setup() function. */
LinkedList *list;
Parser *parser;
Client *client;
Node *head;


/**
 * Setup the sockets and stuff.
 */
void setup()
{
	list = createList();
	parser = createParser();
	head = (Node *)malloc(sizeof(Node));
	setHead(head); // sets the head for monitor.c

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
 * Each successful connection should result in a new Client (Client.c) being added
 * to the linked list (data_structures.c). Information created in this connection
 * is added to the client in the list.
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
        client = createClient(new_sock, sin_size, client_addr, id, curr_usage, min, max, storage_type);
        list->F11(&head, *client); // push client into front of list
        printf("Server: got connection from %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Pushing new SLA to all clients... \n");
        //close(new_sock);
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
    long iops_max = getMaxIOPS();
    long iops_min = getMinIOPS();
    long thru_max = getMaxThru();
    long thru_min = getMinThru();
    long SLA_unused = getUnused();
	snprintf(SLA, sizeof(SLA), \
	    "{\"protocol_version_number\":%d, \
	        \"version\":%lu, \
	        \"client_id\":%lu, \
	        \"storage_type\":%lu, \
	        \"iops_max\":%lu, \
	        \"iops_min\":%lu, \
	        \"throughput_max\":%lu, \
	        \"throughput_min\":%lu, \
	        \"SLA_unused\":%lu}",
	    protocol_version, SLA_version, client_id, storage_type, iops_max, iops_min, thru_max, thru_min, SLA_unused);
}

/****************************************************************
 * Push an SLA update to all connected clients.
 * The socket information associated with each client should
 * be contained in the Client struct (defined in Client.c).
 * That information is extracted and used in the send()
 * function, one client at a time.
 ***************************************************************/
void pushSLA(){
	long cli_ID = NULL;
	long storage_type = NULL;
	Node* current = head;
	while(current != NULL){
		//////////////////////////////////////////////
		int temp_sock = (current->c).sockfd;
		socklen_t temp_size = (current->c).address_size;
		struct sockaddr_in temp_addr = (current->c).address;
		if (connect(temp_sock, (struct sockaddr *)&temp_addr, temp_size) == 0){
		//////////////////////////////////////////////
			cli_ID = (current->c).id;
			storage_type = (current->c).storage_type;
			makeSLA(1, cli_ID, storage_type);
			send(temp_sock, SLA, sizeof(SLA), 0);
			current = current->next;
		}
		close(temp_sock);
	}
}

/**************************************************************
 * Free all of the dynamically allocated memory
 * so that we don't have a memory leak.
 *************************************************************/
void closeProgram(){
	list->ds_freeList(head);
	free(list->head);
	free(list);
	free(parser);
}

/**************************************************************
 * Go through the clients and close their associated sockets
 * so that we don't have a socket leak.
 *************************************************************/
void closeConnections(){
	Node* current = head;
	while(current != NULL){
		close((current->c).sockfd);
		current = current->next;
	}
}

/*********************************************************************
 * This is the main program.
 * 1. It initializes the sockets with the setup() function.
 * 2. It continually waits for clients to make connection requests.
 * 3. If it gets a connection, it sends an SLA update to all clients.
 ********************************************************************/
int main(void){
    setup();
    while(1){
		if(getClient()){
			pushSLA();
		}
    }
    closeConnections();
    closeProgram();
    return 0;
}























