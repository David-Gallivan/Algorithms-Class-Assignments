//JMJ
/*
 * HEAP FUNCTIONS
 * David Gallivan
 */

#include"string.h"

//Logos is greek for "word."
struct logos {
	char* vocabulum;  //Latin for "word"
	int numerus;                //"count"
	struct logos* proximum;     //"next"
};

//ISSHIFTED
//Returns 1 if the first entry of the array is null
//Otherwise returns 0
//This helps verify that the heap is aligned properly
int isShifted(struct logos** heap){
	if(heap[0])
		return 0;
	return 1;
}


//COMPARE
//compares logos structs based on "condition"
//if condition is 0, the comparison is by alphabetical order
//if     "        1, it is by number of occurrences
//TODO make sure it it "oriented" properly!
//TODO when testing & debugging I will do so!
int compare(struct logos* l1, struct logos* l2, int condition){
	if(condition == 0)
		return -strcmp(l1->vocabulum, l2->vocabulum);
	if(condition == 1){
		if(l1->numerus > l2->numerus)
			return 1;
		else 
			return 0;
	}
	return 42;
}

//SMALLERCHILD
//returns the index of the smaller child of an element
int smallerChild(struct logos** heap, int lastindex, int condition, int currentindex){
	int answ = currentindex * 2;
	if(answ+1 <= lastindex)
		if(compare(heap[answ], heap[answ+1], condition) > 0)
			answ++;
	return answ;
}

//SIFTDOWN
void siftDown(struct logos** heap, int lastindex, int condition, int currentindex){
	int childindex = 0;
	while(2*currentindex <= lastindex){
		childindex = smallerChild(heap, lastindex, condition, currentindex);
		if (compare(heap[childindex], heap[currentindex], condition) > 0)
			return;
		struct logos* temp = heap[currentindex];
		heap[currentindex] = heap[childindex];
		heap[childindex] = temp;
		currentindex = childindex;
	}
}

//REMOVE
//removes the top element and returns it
//resifts as needed
struct logos* deleteTop(struct logos** heap, int *lastindex, int condition){
	struct logos* answ = heap[1];
	heap[1] = heap[*lastindex];
	*lastindex -= 1;
	siftDown(heap, *lastindex, condition, 1);
	return answ;
}

//HEAPSORT
//assumes that the array given already has pseudo-data
//hence, ignores array[0]
//sorts the array based on condition - see compare()
//when finished, array[0] is still empty
void heapSort(struct logos** heap, int *lastindex, int condition){
	//first, heapify
	for(int i = *lastindex/2; i > 0; i--)
		siftDown(heap, *lastindex, condition, i);
	//after heapifying, put in order
	for(int i = *lastindex; i > 0; i--)
		heap[i] = deleteTop(heap, lastindex, condition);
	heap += sizeof(struct logos*);
}
