//JMJ
//
//--------------------------------------------
//  CS315, Spring 2019
//  Assignment 1 : the Secretary
//
//  main.cpp
//
//  David Patrick Gallivan
//  Begun 01/21/2019, finished 01/30/2019
//--------------------------------------------

//CONSTANT VARS
const float QUITTIN_TIME = 10.0;
const float QMAX = 1.0;
const float DMAX = 2.0;
const float PMAX = 2.0;
const float PENALTY = 0.3;
const int VISITOR = 1;
const int CALLER = 2;
const int FINISHED = 3;

//INCLUSIONS
#include "stdio.h"
#include "stdlib.h"
#include "officeQueue.h"

//FUNCTION PROTOTYPES
void addVisitor(int index, struct officeQueue *kyu);
void addCaller(int index, struct officeQueue *kyu);
int answerClient(struct officeQueue *kyu);
void printLogEntry(float currentTime, int event, int clientIndex, struct officeQueue *kyu);

//MAIN FUNCTION
int main() {
	float currentTime = 0.0;
	float q = (QMAX * (float) random()/RAND_MAX);
	float p = (PMAX * (float) random()/RAND_MAX);
	float D = PMAX + QMAX;
	float *d = &D;  //this way, d is never the very first event chosen
	int clientIndex = 1;  //index of the client about to appear
	struct officeQueue *kyu = createOfficeQueue();
	printf("Here is today's office log:\n");
	printf("---------------------------\n");
	while(currentTime < QUITTIN_TIME){
		if(q <= p && q <= *d){  //the <= operator means that ties are broken in favor of q
			currentTime += q;
			p -= q;
			*d -= q;
			addVisitor(clientIndex, kyu);
			printLogEntry(currentTime, VISITOR, clientIndex, kyu);
			q = (QMAX * (float) random()/RAND_MAX);
			clientIndex += 1;
		}
		else if(p <= *d){  //ties are broken in favor of p
			currentTime += p;
			q -= p;
			*d -= p;
			addCaller(clientIndex, kyu);
			printLogEntry(currentTime, CALLER, clientIndex, kyu);
			p = (PMAX * (float) random()/RAND_MAX);
			clientIndex += 1;
		}
		else{
			currentTime += *d;
			q -= *d;
			p -= *d;
			int isVisitor = kyu->front->next->priority;
			printLogEntry(currentTime, FINISHED, answerClient(kyu), kyu);
			if(!isVisitor){  //i.e. if this is a caller  //extra credit code
				float pen = (PENALTY * (float) random()/RAND_MAX);
				currentTime += pen;
				q -= pen;
				p -= pen;
				printf("Flustered secretary: waiting %f sec, continuing at %f\n", pen, currentTime);
			}
		}
		if(isEmpty(kyu))
			{d = &D; D = QMAX + PMAX;}  //keeps d high if empty list so p or q will be next
		else
			d = &(kyu->front->next->d);  //we are always "helping" the front client	
	}
	printf("---------------------------\n");
	printf("All done!\n");
	return 0;
}  //main function

void addVisitor(int i, struct officeQueue *kyu){
	float d = (DMAX * (float) random()/RAND_MAX);
	int priority = 1;  //This differentiates visitors from callers
	insertRear(i, d, priority, kyu);
	kyu->visitorCount++;
	return;
} //addVisitor

void addCaller(int i, struct officeQueue *kyu){
	float d = (DMAX * (float) random()/RAND_MAX);
	int priority = 0;  //This differentiates visitors from callers
	insertFront(i, d, priority, kyu);
	kyu->callerCount++;
	return;
}  //addCaller

int answerClient(struct officeQueue *kyu){
	int index = kyu->front->next->index;
	if(kyu->front->next->priority)  //i.e. if this is a visitor
		kyu->visitorCount--;
	else
		kyu->callerCount--;
	removeFront(kyu);
	return index;
}  //answerClient

void printLogEntry(float currentTime, int event, int clientIndex, struct officeQueue *kyu){
	printf("%f sec:\t", currentTime);
	if(event == 1) printf("Client #%d walked in\t", clientIndex);
	else if(event == 2) printf("Client #%d called\t", clientIndex);
	else if(event == 3) printf("Client #%d was answered\t", clientIndex);
	printf("\tClients waiting: %d\t", kyu->callerCount + kyu->visitorCount);
	printf("Callers waiting: %d  Visitors waiting: %d\n", kyu->callerCount, kyu->visitorCount);
}  //printLogEntry
