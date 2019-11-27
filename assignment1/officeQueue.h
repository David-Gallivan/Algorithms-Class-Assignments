//JMJ
//
//----------------------------------------
//  CS315, Spring 2019
//  Assignment 1 : the Secretary
//
//  officeQueue.h
//
//  David Patrick Gallivan
//  Begun 01/21/2019, Finished 01/30/2019 
//----------------------------------------

#include"stdlib.h"

const int DUMMY;

//Client node
struct client {
	float d;
	int index;
	int priority;
	struct client *next;
};

//Queue handle
struct officeQueue {
	struct client *front;
	struct client *rear;
	int callerCount;
	int visitorCount;
};

//Function prototypes
void *createOfficeQueue();
void printQueue(struct officeQueue *kyu);  //for debugging purposes
void insertRear(int i, float d, int priority, struct officeQueue *kyu);
void insertFront(int i, float d, int priority, struct officeQueue *kyu);
void removeFront(struct officeQueue *kyu);
int isEmpty(struct officeQueue *kyu);  //returns 1 if empty, else 0
