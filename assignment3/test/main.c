/*JMJ
 *
 * Oaxaca Wordcounter
 * 	by David Gallivan
 * 
 * CS315, Univ. of KY
 * 7 March, 2019
 *
 * MAIN_C
 */

#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"ctype.h"
#include"heap.c"

#define HTABLE_SIZE 512
#define SPACE 0x20

size_t MAX_WORD_LEN = 64;

//note that the logos struct is defined in heap.c
struct logos* htable[HTABLE_SIZE];

int storeWord(char* word);
struct logos* makeLogos(char* word);
unsigned int oaxacaHash(char* word);
struct logos* isHere(char* word, unsigned int hval);

//MAIN FUNCTION
int main(){
	//initialization
	for(int i = 0; i < HTABLE_SIZE; i++)
		htable[i] = NULL;
	char* word;
	word = (char*) malloc(MAX_WORD_LEN);
	char ch;
	int i = 0;
	int wordcount = 0;  //counts duplicates each time they occur
	//below loop inputs words and stores them in the hash table
	while((ch = fgetc(stdin))){
		if(isalnum(ch)){  //only copies letters & numbers into buffer
			word[i] = ch;
			i++;
		}
		else if(ch!=EOF){
			word[i] = 0;
			if(i) {
				storeWord(word);
				//printf("stored %s at htable[%d]\n", word, storeWord(word));
				wordcount++;
				word[0] = 0;
			}
			i = 0;
		}
		else {  //i.e. upon an EOF
			if(word[0]) {  //i.e., if we are in the process of reading a word
				word[i] = 0;
				storeWord(word);
				//printf("stored %s at htable[%d]\n", word, storeWord(word));
				wordcount++;
			}
			break;
		}
	}
	//now for heap stuff
	struct logos* heap[wordcount+1];  //the heap is just an array, and we will operate on it with heap functions. heap[0] is pseudo-data.
	heap[0] = 0;
	int heapindex = 1;
	struct logos* thislogos;
	for(i = 0; i < HTABLE_SIZE; i++){
		if(htable[i]){
			heap[heapindex] = thislogos = htable[i];
			//printf("transferred %s\t%d\n", heap[heapindex]->vocabulum, heap[heapindex]->numerus);
			heapindex++;
			while(thislogos->proximum){
				heap[heapindex] = thislogos->proximum;
				thislogos = thislogos->proximum;
				//printf("transferred %s\t%d\n", heap[heapindex]->vocabulum, heap[heapindex]->numerus);
				heapindex++;
			}
		}
	}
	int *lastIndex = malloc(sizeof(int));
	*lastIndex = heapindex-1;
	int numEntries = *lastIndex;
//	printf("%d\n", isShifted(heap));
	heapSort(heap, lastIndex, 0);  //alphabetical
	for(heapindex = 1; heapindex <= numEntries; heapindex++)
		printf("%s %d\n", (*heap[heapindex]).vocabulum, (*heap[heapindex]).numerus);

	*lastIndex = numEntries;
	//printf("%d\n", *lastIndex);
	heapSort(heap, lastIndex, 1);  //reverse numeric
	printf("\n");
	for(heapindex = 1; heapindex <= numEntries; heapindex++)
		printf("%s %d\n", (*heap[heapindex]).vocabulum, (*heap[heapindex]).numerus);

	return 0;
}//MAIN FUNCTION


//STOREWORD
//Either puts the current word into hash table
//or updates its existing entry
int storeWord(char* word){
	//printf("storing %s\n", word);
	unsigned int hval = oaxacaHash(word);
	struct logos* foundit;
	//printf("made a foundit ptr, looking for it\n");
	foundit = isHere(word, hval);
	//printf("foundit or not: %x", foundit);
	if(foundit)
		foundit->numerus++;
	else {
		if(htable[hval]){
			struct logos* quoit = htable[hval];
			while(quoit->proximum)
				quoit = quoit->proximum;
			quoit->proximum = makeLogos(word);
		}
		else{
			struct logos* vocnov = makeLogos(word);
			htable[hval] = vocnov;
			//htable[hval] = makeLogos(word);
		}
	}
	return hval;
}

//MAKELOGOS
//Initializes a logos struct with the given word
struct logos* makeLogos(char* word){
	//printf("making logos\n");
	struct logos* newLogos = malloc(sizeof(struct logos));
	newLogos->vocabulum = malloc(MAX_WORD_LEN);
	strncpy(newLogos->vocabulum, word, MAX_WORD_LEN);
	newLogos->numerus = 1;  //This is the first time we see this word
	newLogos->proximum = NULL;
	//printf("about to return it\n");
//	struct logos* ptr = &newLogos;
	return newLogos;
}//MAKELOGOS

//OAXACAHASH
//coming soon
//until further notice, only returns the meaning of the universe
//unsigned int oaxacaHash(char* word){
//	return (unsigned int) 42;
//}
unsigned int oaxacaHash(char* word){
	unsigned int hval = 0;
	for(int i = 0; i < strlen(word); i+=2){
		hval += (word[i] % 2);
		hval += (word[i] % 3);
		hval += (word[i] % 5);
		hval += (word[i] % 7);
		if(i % 4 == 0)
			hval -= 32;
	}
	hval *= (HTABLE_SIZE + word[0]);
	hval /= 8;
	hval %= HTABLE_SIZE;
	return hval;
}//OAXACAHASH



//ISHERE
//Given a hash table entry and a word,
//returns a pointer to the word's logos if existing,
//otherwise returns null
struct logos* isHere(char* word, unsigned int hval){
	//printf("looking at %d: %x\n",hval, htable[hval]);
	if(!htable[hval])
		return NULL;
	//printf("Maybe here...\n");
	struct logos* logcation = htable[hval];
	//printf("made a new logosptr, about to loop\n");
	while(logcation){
		//printf("in loop\n");
		if(!strncmp(logcation->vocabulum,word,MAX_WORD_LEN))
			break;
		logcation = logcation->proximum;
	}
	//printf("out of loop\n");
	return logcation;
}//ISHERE

