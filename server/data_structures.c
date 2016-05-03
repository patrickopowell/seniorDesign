#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Client.h"


/* Function typedefs */
typedef void (*f1)(Node **, Client); // push
typedef void (*f2)(Node *, long); // delete
typedef Client (*f3)(Node *, long); // get client by ID
typedef int (*f4)(Node *); // get length
typedef void (*f5)(Node *); // free the list

typedef struct{
	Client c;
	Node *next;
} Node;

/**
 * Creates a linked list of clients.
 * USAGE:
 * 		LinkedList *list = createList(client);
 */
typedef struct {
	Node *head;
    f1 F1; // push client to front of list
    f1 F2; // delete client by ID
    f1 F3; // get client by ID
    f1 F4; // get length of list
    f1 F5; // free the List's memory.
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
	list->F1 = push; // push client to front of list
	list->F2 = delete; // delete client by ID
	list->F3 = getClientByID; // get client by ID
	list->F4 = length; // get list length
	list->F5 = destroyList; // free list memory space
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
	if(curr->c->id == id){
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
Client getClientByID(Node *head, long id){
	Client *temp;
	Node *current = head;
	while (current != NULL) {
		if(current->c->id == id){
			temp = current->c;
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
















