#ifndef BOOL_H
#define BOOL_H
typedef enum { false, true } bool;
#endif

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "Client.h"
#include <stdlib.h>

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

typedef struct {
	Node *head;
    f11 ds_push; // push client to front of list
    f12 F12; // delete client by ID
    f13 F13; // get client by ID
    f14 ds_getLength; // get length of list
    f15 ds_freeList; // free the List's memory.
} LinkedList;

//LinkedList *createList(Client *);
LinkedList *createList();

#endif






