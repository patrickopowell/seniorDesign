#ifndef BOOL_H
#define BOOL_H
typedef enum { false, true } bool;
#endif

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <Client.h>

typedef struct{
	Client c;
	Node *next;
} Node;

typedef struct {
	Node *head;
    f1 F1; // push client to front of list
    f1 F2; // delete client by ID
    f1 F3; // get client by ID
    f1 F4; // get length of list
    f1 F5; // free the List's memory.
} LinkedList;

//LinkedList *createList(Client *);
LinkedList *createList();

void push(Node **, Client);
void delete(Node *, long);
Client getClientByID(Node *, long);
int length(Node*);
destroyList(Node *);

#endif






