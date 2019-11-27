/*JMJ
 *
 * CS315 :  Assignment 4
 * FAMILY TREES
 * by David Gallivan
 *
 * 5 April, 2019
 *
 */

#include"stdlib.h"
#include"stdio.h"
#include"string.h"

size_t MAX_COMMAND_LEN = 100;

/* LINKED LIST STRUCT AND FUNCTIONS */
//note: this list considers data==0 to mean an empty node

//linked list (recursive)
struct list {
	int data;
	struct list* next;
};

//print_list
void print_list(struct list* l){
	printf("%d", l->data);
	if(l->next){
		printf(" ");
		print_list(l->next);
	}
}//print_list

//make_list
struct list* make_list(){
	struct list* l = malloc(sizeof(struct list));
	l->data = 0;
	l->next = 0;
	return l;
}//make_list

//list_insert
void list_insert(struct list* l, int data){
	if(!l->next){
		if(!l->data)
			l->data = data;
		else{
			l->next = make_list();
			l->next->data = data;
		}
	}
	else
		list_insert(l->next, data);
}//list_insert

//search_list
int search_list(struct list* l, int quarry){
	if(l->data == quarry)
		return 1;
	if(l->next)
		return search_list(l->next, quarry);
	else
		return 0;
}//search_list

/* QUEUE STRUCTS AND FUNCTIONS */
//This data structure is used for "Relate"

//node
struct Qnode {
	int data;
	struct Qnode* next;
};

//handle
struct Queue {
	struct Qnode* front;
	struct Qnode* rear;
};

//makeQ
struct Queue* makeQ(){
	struct Queue* kyu = malloc(sizeof(struct Queue));
	struct Qnode* dummy = malloc(sizeof(struct Qnode));
	dummy->data = -1;
	dummy->next = 0;
	kyu->front = kyu->rear = dummy;
	return kyu;
}//makeQ

//insertQ
void insertQ(struct Queue* kyu, int data){
	struct Qnode* newNode = malloc(sizeof(struct Qnode));
	newNode->data = data;
	newNode->next = 0;
	kyu->rear->next = newNode;
	kyu->rear = newNode;
}//insertQ

//isEmpty
//returns 1 if empty, 0 if not empty
int isEmptyQ(struct Queue* kyu){
	if(kyu->front->next) return 0;
	return 1;
}//isEmpty

//deleteQ
//returns data from the first non-dummy node and deletes said node
int deleteQ(struct Queue* kyu){
	int retval = kyu->front->next->data;
	if(isEmptyQ(kyu))
		return -1;
	kyu->front->next = kyu->front->next->next;
	if(isEmptyQ(kyu))
		kyu->rear = kyu->front;
	return retval;
}//deleteQ

/* STRUCTS AND ARRAYS FOR FAMILY DATA STORAGE */

//person struct
struct person {
	int parent;
	int marriage;  //I have chosen to allow only one marriage for now
};

//family struct
struct family {
	int husband;
	int wife;
	struct list* children;
};

//person array
struct person* peeps[100];
//family array
struct family* fams[100];

/* COMMAND IMPLEMENTATION FUNCTIONS */

//insert_person
//implements the "Insert" command
void insert_person(char** tokens){
	int ID = atoi(tokens[1]);
	int parent = atoi(tokens[3]);
	int marriage = atoi(tokens[5]);
	peeps[ID] = malloc(sizeof(struct person));
	peeps[ID]->parent = parent;
	peeps[ID]->marriage = marriage;
	printf("Inserted individual %d with parents %d and marriage %d\n", ID, peeps[ID]->parent, peeps[ID]->marriage);
}//insert_person

//insert_family
//implements the "Family" command
void insert_family(char** tokens){
	int ID = atoi(tokens[1]);
	int Husband = atoi(tokens[3]);
	int Wife = atoi(tokens[5]);
	struct list* child_list = make_list();
	int childID;
	for(int i = 7; i < 25; i+=2){
		if(tokens[i]){  //i.e. if there is such a token
			if((childID = atoi(tokens[i])))  //i.e. if this is a nonzero int
				list_insert(child_list, childID);
			else break;
		}
		else break;
	}
	fams[ID] = malloc(sizeof(struct family));
	fams[ID]->husband = Husband;
	fams[ID]->wife = Wife;
	fams[ID]->children = child_list;
	printf("Inserted family %d with husband %d, wife %d, and children ", ID, fams[ID]->husband, fams[ID]->wife);
	print_list(fams[ID]->children);
	printf("\n");
}//insert_family

//check_children
//supporting function for check()
//checks child pointers for back pointers
int check_children(struct list* child, int fam){
	int error_count = 0;
	if(child->data){
		if(!peeps[child->data]) {
			printf("Family %d points to child %d, who does not exist in the data\n",fam,child->data);
			error_count++;
		}
		else if(peeps[child->data]->parent != fam){
			printf("Family %d points to child %d, but there is no backpointer\n",fam,child->data);
		error_count++;
	}
	}
	if(child->next)
		error_count += check_children(child->next, fam);
	return error_count;
}//check_children

//check_data_consistency
//checks all relationships for mutuality
//implements the "Check" function
void check_data_consistency(){
	int i, parent, marriage, husband, wife, error;
	error = 0;  //i.e. error flag is initially false
	/* first we check all the people */
	for(i=0; i<100; i++){
		if(peeps[i]){
			parent = peeps[i]->parent;
			marriage = peeps[i]->marriage;
			if(fams[parent]){
				if(!search_list(fams[parent]->children, i))
				{printf("Individual %d points to parent family %d, but there is no backpointer\n",i,parent); error = 1;}
			} 
			else
			   if(parent)  //this if ensures that family 0 is treated as unknown and thus ok
			   {printf("Individual %d points to parent family %d, which does not exist in the data\n",i,parent); error = 1;}
			if(fams[marriage]){
				if(fams[marriage]->husband != i && fams[marriage]->wife != i)
				{printf("Individual %d points to marriage %d, but there is no backpointer\n",i,marriage); error = 1;}
			}
		    else
				if(marriage)  //marriage 0 is ok
				{printf("Individual %d points to marriage %d, which does not exist in the data\n",i,marriage); error = 1;}
		}
	}
	/* then we check all the families */
	for(i=0; i<100; i++){
		if(fams[i]){
			husband = fams[i]->husband;
			wife = fams[i]->wife;
			if(peeps[husband]){
				if(peeps[husband]->marriage != i)
				{printf("Family %d points to husband %d, but there is no backpointer\n",i,husband); error = 1;}
			}
			else
				if(husband)
				{printf("Family %d points to husband %d, who does not exist in the data\n",i,husband); error = 1;}
			if(peeps[wife]){
				if(peeps[wife]->marriage != i)
				{printf("Family %d points to wife %d, but there is no backpointer\n",i,wife); error = 1;}
			}
			else
				if(wife)
				{printf("Family %d points to wife %d, who does not exist in the data\n",i,wife); error = 1;}
			//Now we have to check the children
			if(fams[i]->children)
				error += check_children(fams[i]->children, i);
		}
	}
	if(!error) printf("The data are consistent\n");
}//check_data_consistency


//pair: used for the chain in relate_two_individuals()
struct pair{
	int a;
	int b;
};

//relate_sub1
//subfunction of relate_two_individuals, below
int relate_sub1(int there, int* visited, struct Queue* Q, struct pair* chain, int* chainIndex, int place, int destination){
	if(!(there%100))  //i.e. if this is peeps[0] or fams[0], ignore it
		return 0;
	if(!visited[there]){
		visited[there] = 1;
		insertQ(Q, there);
		chain[*chainIndex].a=place;
		chain[*chainIndex].b=there;
		*chainIndex += 1;	
		if(there == destination)
			return 1;
	}
	return 0;
}//relate_sub1

//relate_two_individuals
//finds the shortest path through the graph between two individuals
//implements the "Relate" command
//uses an adapted breadth-first search
void relate_two_individuals(char** tokens){
	int here, destination, place, chainIndex, foundit;
	here = atoi(tokens[1]);
	destination = atoi(tokens[2]);
	if(!(peeps[here] && peeps[destination])){
		printf("One of those people does not exist!\n");
		return;
	}
	struct list* thisChild;
	struct pair chain[200];
	chainIndex = 0;
	int visited[200];  //0-99 <--> peeps[0-99]; 100-199 <-->fams[0-99]
	for(int i = 0; i<200; i++) visited[i]=0;
	visited[here] = 1;
	struct Queue* Q = makeQ();
	insertQ(Q, here);
	/* Now that everything is initialized, we do the BFS and record our steps with chain[] */
	while(!isEmptyQ(Q)){
		place = deleteQ(Q);
		if(place<100){  //i.e. if this is a person
			if((foundit = relate_sub1(peeps[place]->parent+100, visited, Q, chain, &chainIndex, place, destination))) break;
			if((foundit = relate_sub1(peeps[place]->marriage+100, visited, Q, chain, &chainIndex, place, destination))) break;
		}
		else{
			if((foundit = relate_sub1(fams[place-100]->husband, visited, Q, chain, &chainIndex, place, destination))) break;
			if((foundit = relate_sub1(fams[place-100]->wife, visited, Q, chain, &chainIndex, place, destination))) break;
			//now for those pesky children! No recursion this time!
			thisChild = fams[place-100]->children;
			while(thisChild){
				if((foundit = relate_sub1(thisChild->data, visited, Q, chain, &chainIndex, place, destination))) break;
				thisChild = thisChild->next;
			}
			if(foundit) break;
		}
	}
	/* Now we report our results */
	if(foundit){
		chainIndex--;  //this gives us the index of the last pair in the chain
		struct pair path[200];
		int pathIndex = 199;
		path[pathIndex] = chain[chainIndex];
		chainIndex--;
		while(chainIndex >= 0){  //loop copies all relevant, non-redundant elements from chain to path
			if(chain[chainIndex].b==path[pathIndex].a){
				pathIndex--;
				path[pathIndex] = chain[chainIndex];
			}
			chainIndex--;
		}
		if(path[pathIndex].a > 99)
			printf("family %d ", path[pathIndex].a-100);
		else
			printf("individual %d ", path[pathIndex].a);
		while(pathIndex < 200){
			if(path[pathIndex].b > 99)
				printf("family %d ", path[pathIndex].b-100);
			else
				printf("individual %d ", path[pathIndex].b);
			pathIndex++;
		}
		printf("\n");
	}
	else{
		printf("Individuals %d and %d are not related\n", here, destination);
	}
}//relate_two_individuals

/* INPUT PARSING & MAIN */

//evaluate_command
//tokenizes an input line and executes it if it is a command
void evaluate_command(char* command){  //TODO magic nums
	/* First we tokenize the command */
	char* tokens[25];
	char* tok = malloc(100);
	int i=0, j=0, k=0;
	for(; i<25; i++)  //initializes tokens
		tokens[i] = 0;
	for(i = 0; i < strnlen(command, MAX_COMMAND_LEN); i++ ){
		if(command[i]==0x0a)  //newline ends command
			break;
		if(command[i]==0x20){  //space delimits
			tokens[k] = tok; k++;
			tok = malloc(100); j=0;
			continue;
		}
		else{
			tok[j] = command[i];
			j++;
		}
	}
	tokens[k] = tok;
	/* Then we identify the command and call its function */
	if(!strncmp(tokens[0], "Insert", 100))
		insert_person(tokens);
	else if(!strncmp(tokens[0], "Family", 100)){
		insert_family(tokens);
	}
	else if(!strncmp(tokens[0], "Check", 100))
		check_data_consistency();
	else if(!strncmp(tokens[0], "Relate", 100))
		relate_two_individuals(tokens);
}//evaluate_command

//MAIN FUNCTION
int main(){
	for(int i = 0; i < 100; i++) {
		peeps[i] = 0;
		fams[i] = 0;
	}
	char* buf = malloc((int)MAX_COMMAND_LEN);
	while(getline(&buf, &MAX_COMMAND_LEN, stdin) >=0){
		evaluate_command(buf);
	}
	return 0;
}//main
