/**
 * This is the interface that will communicate with the client.
 */

#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080 // The port users will connect to.
// Check out raw sockets
int main(void)
{
    /*********************************************************************************************
     * struct sockaddr_in is defined in /usr/include/netinet/in.h
     * It defines a basic Internet socket address. 
     * It holds:
     *  - Port Number
     *  - Internet Address
     *  - Size of struct sockaddr
     * There is another struct called "sockaddr" which describes
     * a generic socket address.
     ********************************************************************************************/
    struct sockaddr_in local_addr; // my address
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT);
    local_addr.sin_addr.s_addr = 0;
    memset(&(local_addr.sin_zero), '\0', 8); // Zero rest of struct
    struct sockaddr_in client_addr; // client address
    /********************************************************************************************
     * The following is the buffer for what I'm sending.
     * And other variables.
     *******************************************************************************************/
    //char buffer[1024];
    socklen_t sin_size = NULL;
    /*********************************************************************************************
     * The following are integer representations of socket file descriptors.
     ********************************************************************************************/
    //int sock; // Socket for listening.
    int new_sock; // Socket for connect()/accept()
    ////////////////////////////////////////////////////////////////////////////////////////
	
	int domain = PF_INET;
	int type = SOCK_STREAM;
	int protocol = 0;
	int sock = socket(domain, type, protocol); // create a socket
	if (sock < 0){
	    printf("Error opening socket! \n");
	}
	
	int yes = 1;
	/* Allows us to use this socket for binding even if it seems to be in use, already. */
	int sockOption = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	if(sockOption == -1){
        printf("Error setting socket option to SO_REUSEADDR \n");
    }
	
	//struct sockaddr *host_addr = ;
	socklen_t addr_length = sizeof(struct sockaddr);
	int binding = bind(sock, (struct sockaddr *) &local_addr, addr_length);
	if(binding != 0){
	    //error
	}
	
	
	int bl_size = 5; //num_clients; // num_clients. backlog size. Listens for this many connections.
	int listening = listen(sock, bl_size);
	if(listening != 0){
	    //error
	}
	
	//sockaddr *cliAddr = ; // address of the client
	//socklen_t *cliAddr_len = ; // client address length
	//new_sock = accept(sock, cliAddr, cliAddr_len); // returns a new socket file descriptor
	
	////////////////////////////////////Start project specifics ////////////////////////////////////
	int SLA_min = NULL;
	int SLA_unused = NULL;
	// Start accepting connections
	for(int i = 0; i < 5; i++){
	    sin_size = sizeof(struct sockaddr_in);
	    new_sock = accept(sock, (struct sockaddr *) &client_addr, &sin_size); // returns new socket file-descriptor
	    if(new_sock < 0){
	        //error
	    } else{
	        incrementNumClients();
	        printf("Server: got connection from %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	        SLA_min = getSLA_min();
	        SLA_unused = getUnused();
	        send(new_sock, "{SLA_min: 2, SLA_unused:3\n}", 25, 0);
	    }
	}
	printf("Hello World!\n");
    return 0;
}


































