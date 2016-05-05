//#include "monitor.h"
#include "data_structures.h"
#include <stdio.h>

int numClients = 1;
long bandwidth = 10000;
long arbitrary = 0;
long usage = 0;
long SLA_min = 0;
long unused = 0;
long SLA_version = 0;
Node *head;

/* Sets the head node of a linked list. */
void setHead(Node *front){
	head = front;
}

/* Increment the number of connected clients. */
void incrementNumClients(){
	numClients++;
}
/* Decrement the number of connected clients. */
void decrementNumClients(){
	numClients--;
}

/*Increments the SLA version.*/
void increment_SLA(){
	SLA_version++;
}

/************************* GETTERS **********************************/

/*Returns the current SLA version.*/
long getSLA_version(){
	return SLA_version;
}

/* Return the number of connected clients. */
int getNumClients(void){
	return numClients;
}

/* Return the maximum bandwidth capacity. */
long getBandwidth(void){
	return bandwidth;
}

/* Return arbitrary maximum bandwidth of client. */
long getArbitrary(void){
	return 0;
}

/* Return bandwidth being used. */
long getUsage(Node *head){
	long temp = NULL;
	Node* current = head;
	while(current != NULL){
		temp += (current->c).current_usage;
		current = current->next;
	}
	return temp;
}

/* Return unused bandwidth. */
long getUnused(void){
	long temp = NULL;
	temp = bandwidth - getUsage(head);
	return temp;
}
    
/////////////////// New as of March 15, 2016 /////////////////////
/**********************************************************************
 * Clients are guaranteed at least this much bandwidth,
 * without interference.
 *********************************************************************/
long getMinThru(void){
	return bandwidth / numClients;
}

/**********************************************************************
 * This is whatever is automatically allocated plus anything the other
 * clients aren't currently using.
 *********************************************************************/
long getMaxThru(void){
	long temp = NULL;
	temp = getMinThru() + getUnused();
	return temp;
}

long getMinIOPS(void){
	return bandwidth / numClients;
}
long getMaxIOPS(void){
	long temp = NULL;
	temp = getMinThru() + getUnused();
	return temp;
}
////////////////////// End March 15, 2016 ////////////////////////
    
    
    
    

/********************** Watch for client compliance. *******************/

/* Alert if unused bandwidth is less than zero. */
int alert(int unused){
	if(unused < 0){
		return 1;
	} else{
		return 0;
	}
}
    
    
    
    
    
    
    
