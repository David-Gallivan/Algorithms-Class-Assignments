//JMJ
//
//----------------------------------------
//  CS315, Spring 2019
//  Assignment 1 : the Secretary
//
//  officeQueue.cpp
//
//  David Patrick Gallivan
//  Begun 01/21/2019, Finished 01/20/2019
//----------------------------------------

#include"stdio.h"
#include"stdlib.h"
#include"officeQueue.h"

const int DUMMY = -1;

void *createOfficeQueue(){
	struct officeQueue *kyu = malloc(sizeof(struct officeQueue));
	struct client *dummy = malloc(sizeof(struct client));
	dummy->index = dummy->priority = DUMMY;
	dummy->d = 123456789;  //superlatively big - should never be used
	dummy->next = 0;  //NULL
	kyu->front = kyu->rear = dummy;
	return kyu;
}  //createOfficeQueue

void printQueue(struct officeQueue *kyu){
	printf("Queue printout:\n");
	printf("\tCounts :  %d %d\n", kyu->callerCount, kyu->visitorCount);
	//printf("\tDummy d, next :  %f %x\n", kyu->front->d, kyu->front->next);
	struct client *thisOne = kyu->front;
	while(thisOne->next){
		printf("\tClient %d: %f %d %d %x\n", thisOne->index, thisOne->d, thisOne->index, thisOne->priority, thisOne->next);
		thisOne = thisOne->next;
	}
	printf("\tClient %d: %f %d %d %x\n", thisOne->index, thisOne->d, thisOne->index, thisOne->priority, thisOne->next);

	printf("end of Queue\n");
}  //printQueue

void insertRear(int i, float d, int priority, struct officeQueue *kyu){
	struct client *newClient = malloc(sizeof(struct client));
	newClient->d = d;
	newClient->index = i;
	newClient->priority = priority;
	newClient->next = 0;  //NULL
	kyu->rear->next = newClient;
	kyu->rear = newClient;
}  //insertRear

void insertFront(int i, float d, int priority, struct officeQueue *kyu){
	struct client *newClient = malloc(sizeof(struct client));
	newClient->d = d;
	newClient->index = i;
	newClient->priority = priority;
	if(isEmpty(kyu))
		kyu->rear = newClient;
	newClient->next = kyu->front->next;  //keeping the dummy in mind!
	kyu->front->next = newClient;
}  //insertFront

void removeFront(struct officeQueue *kyu){
	if(isEmpty(kyu))  //empty queue case
		return;
	kyu->front->next = kyu->front->next->next;
	if(isEmpty(kyu))  //one-client case
		kyu->rear = kyu->front;
}  //removeFront

int isEmpty(struct officeQueue *kyu){
	if(kyu->front->next) return 0;
	return 1;
}  //isEmpty
