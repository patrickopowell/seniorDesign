#include <stdio.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "data_structures.h"
#include "Client.h"


void test_LinkedList_Push(void){
	int temp = -1;
	Client *client = createClient(NULL, NULL, NULL, 1, 1, 1, 1, 1);
	Node *head = (Node *)malloc(sizeof(Node));
	LinkedList *list = createList();
	temp = list->ds_getLength();
	CU_ASSERT(temp == 0);
	CU_ASSERT(qos_can_send(&rb) == 0);
}










