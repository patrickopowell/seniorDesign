#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "Client.h"

typedef struct Node{
	Client c;
	struct Node *next;
} Node;

void push(Node **, Client);
Node *delete(Node *, long);
Client *getClientByID(Node *, long);
int length(Node*);
void destroyList(Node *);

/* Function typedefs */
typedef void (*f11)(Node **, Client); // push
typedef Node *(*f12)(Node *, long); // delete
typedef Client *(*f13)(Node *, long); // get client by ID
typedef int (*f14)(Node *); // get length
typedef void (*f15)(Node *); // free the list


/**
 * Creates a linked list of clients.
 * USAGE:
 * 		LinkedList *list = createList(client);
 */
typedef struct LinkedList{
	Node *head;
    f11 F11; // push client to front of list
    f12 F12; // delete client by ID
    f13 F13; // get client by ID
    f14 F14; // get length of list
    f15 F15; // free the List's memory.
} LinkedList;

/********************************************************
 * Creates a linked list of Clients,
 * with a head node containing Client c.
 *******************************************************/
//LinkedList *createList(Client *c){
LinkedList *createList(){
	LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
	list->head = malloc(sizeof(Node));
	//list->head->c = c;
	list->F11 = push; // push client to front of list
	list->F12 = delete; // delete client by ID
	list->F13 = getClientByID; // get client by ID
	list->F14 = length; // get list length
	list->F15 = destroyList; // free list memory space
	return list;
}

/**
 * Adds a Client to the front of the list.
 */
void push(Node **head, Client c1){
	Node *newNode = malloc(sizeof(Node)); // create new Node.
	//newNode->c = malloc(sizeof(Client));
	newNode->c = c1; // Set its data.
	newNode->next = *head; // Make it point to head, so it is now first.
	*head = newNode; // Make head first again.
}

/**
 * Removes a client with ID "id" from the list.
 */
Node *delete(Node *curr, long id){
	if(curr == NULL){
		return NULL;
	}
	if((curr->c).id == id){
		Node *temp = curr->next;
		free(curr);
		return temp;
	}
	curr->next = delete(curr->next, id);
	return curr;
}

/**
 * Return a pointer to the client with "id."
 */
Client *getClientByID(Node *head, long id){
	Client *temp;
	Node *current = head;
	while (current != NULL) {
		if((current->c).id == id){
			temp = &(current->c);
			return temp;
		}
		current = current->next;
	}
	return NULL;
}

/**
 * Given a linked list head pointer, compute
 * and return the number of nodes in the list.
 */
int length(Node* head) {
	int count = 0;
	Node* current = head;
	while (current != NULL) {
		count++;
		current = current->next;
	}
	return count;
}

/**
 * Goes through the list and frees the memory.
 */
void destroyList(Node * head){
	Node* temp1 = head;
	while (temp1 != NULL) {
		Node* temp2 = temp1;
		temp1 = temp1->next;
		free(temp2);
	}
	free(temp1);
}
















