/*JMJ
 *
 * The Magnificent Markov-o-Matic
 * by David Gallivan
 *
 * TODO I will put some quirky
 * comments here later to make the
 * grader smile and to explain
 * what's what in this file
 *
 */

#include"stdlib.h"
#include"stdio.h"

#define MAX_RULES 1000

size_t MAX_PATTERN_LEN = 20;
size_t MAX_STRING_LEN = 1024;

struct rule{
	char* left;
	char* right;
	int next;
};


//my_strncpy
//
//I hope it works like the original!
//note: it includes the null byte
void my_strncpy(char* dst, char* src, int len){
	int count = 1;
	char* dst_ptr = dst;
	char* src_ptr = src;
	char cary = *src;
	*dst_ptr = cary;
	while(*src_ptr!=0x00 && count<=len){
		dst_ptr++;
		src_ptr++;
		count++;
		*dst_ptr = *src_ptr;
	}
}//my_strncpy

//my_strnlen
//
//Here goes nothing
//This function does not include the null byte
int my_strnlen(char* str, int max){
	int len = 0;
	char* ptr = str;
	while(*ptr!=0x00 && len<=max){
		len++;
		ptr++;
	}
	return len;
}//my_strnlen

//shift_substring_right
//
//copies all characters at substrIndex and beyond to a location
//diff characters to the right.
//leaves a repetition.
//assumes that there is a buffer more than big enough to handle it!
//note that even cutting it close with this assumption could be a problem!
void shift_substring_right(char* string, int substrIndex, int diff){
	char coming_out[diff];
	char going_in[diff];
	int srcIndex = substrIndex;
	int dstIndex = substrIndex+diff;
	int i;
	for(i = 0; i<diff; i++){
			going_in[i] = string[srcIndex + i];
	}
	int done = 0;
	while(!done){
		for(i = 0; i< diff; i++){
			coming_out[i] = string[dstIndex + i];
		}
		for(i = 0; i < diff; i++){
			string[dstIndex+i] = going_in[i];
			if(going_in[i]==0x00){
				done = 1;
				break;
			}
		}
		for(i=0; i<diff; i++){
			going_in[i] = coming_out[i];
		}
		dstIndex += diff;
	}
}//shift_substring_right		

//shift_substring_left
//
//shifts all characters in string left by diff,
//beginning with string[substrIndex], including null byte.
void shift_substring_left(char* string, int substrIndex, int diff){
	register int substr_len = my_strnlen(&string[substrIndex], MAX_STRING_LEN);
	my_strncpy(&string[substrIndex-diff],&string[substrIndex],substr_len);
}//shift_substring_left

//replace
//
//note that this uses the global variable MAX_PATTERN_LEN
void replace_substring(char* string, char* target, int targetIndex, char* impostor){
	int target_len = my_strnlen(target, MAX_PATTERN_LEN);
	int impostor_len = my_strnlen(impostor, MAX_PATTERN_LEN);
	char chary = string[targetIndex + target_len];
	if(target_len==impostor_len){
		my_strncpy(string+targetIndex, impostor, MAX_PATTERN_LEN);
		string[targetIndex + impostor_len] = chary;
	}
	else if(target_len<impostor_len){
		int difference = impostor_len - target_len;
		char chary = string[targetIndex + target_len];
		shift_substring_right(string, targetIndex + target_len, difference);
		my_strncpy(&string[targetIndex], impostor, MAX_PATTERN_LEN);
		string[targetIndex + impostor_len] = chary;
	}
	else{
		int difference = target_len - impostor_len;
		my_strncpy(&string[targetIndex], impostor, MAX_PATTERN_LEN);
		string[targetIndex + impostor_len] = chary;
		shift_substring_left(string, targetIndex + target_len, difference);
	}
}//replace

//read_rules_from_stdin
//
//Rules must be formatted properly and ended with EOF
void read_rules_from_stdin(struct rule** rules){
	char* buf = malloc(MAX_PATTERN_LEN + 1);  // the +1 avoids messing up the border case
	int token_len = getdelim(&buf, &MAX_PATTERN_LEN, '\'', stdin);
	buf[token_len - 1] = 0x00;  //this gets rid of the delimiting character
	int num_rules = atoi(buf);
	if(num_rules == 0){
		printf("Invalid number of rules");
		return;
	}
	for(int i = 0; i < num_rules; i++){
		rules[i] = (struct rule*) malloc(sizeof(struct rule));
		token_len = getdelim(&buf, &MAX_PATTERN_LEN, '\'', stdin);
		buf[token_len - 1] = 0x00;
		rules[i]->left = malloc(MAX_PATTERN_LEN);
		my_strncpy(rules[i]->left, buf, token_len);
		fgetc(stdin);  //this gets us past the leading '
		token_len = getdelim(&buf, &MAX_PATTERN_LEN, '\'', stdin);
		buf[token_len - 1] = 0x00;
		rules[i]->right = malloc(MAX_PATTERN_LEN);
		my_strncpy(rules[i]->right, buf, token_len);
		token_len = getdelim(&buf, &MAX_PATTERN_LEN, '\'', stdin);
		buf[token_len - 1] = 0x00;
		rules[i]->next = atoi(buf);
	}
}//read_rules_from_stdin

//print_rules
//
//prints a nice chart of all rules
void print_rules(struct rule** rules){
	if(rules[0]==0) return;
	printf("num\t\tLeft\t\tRight\t\tNextRule\n");
	int i = 0;
	while(rules[i]){
		printf("%d\t\t%s\t\t%s\t\t\t%d\n",i,rules[i]->left,rules[i]->right,rules[i]->next);
		i++;
	}
	printf("\n");
}//print_rules

//search_for_substring
//
//if the substring is found, returns the index of
//its first occurrence; otherwise, returns -1
int search_for_substring(char* pattern, char* text){
	int pIndex, pLen;
	long foundit = -1;
	pIndex = 0;
	pLen = my_strnlen(pattern, MAX_PATTERN_LEN);
	char cary = pattern[pLen];
	pattern[pLen] = '%';  //impossible character as per assignment specs
	char* tPtr = text;
	do{
		pIndex = 0;
		while(tPtr[pIndex]==pattern[pIndex]){
			pIndex++;
		}
		if(pIndex==pLen) {
			foundit = tPtr - text;
			break;
		}
		tPtr++;
	}while(*tPtr!=0x00);
	pattern[pLen] = cary;
	return (int) foundit;
}//search_for_substring

//execute_rule
//
//executes the current rule
//returns the index of the next rule to execute
int execute_rule(struct rule** rules, int current_rule, char* string){
	int location = search_for_substring(rules[current_rule]->left, string);
	if(location > -1){
		printf("%d\t\t",current_rule);
		replace_substring(string, rules[current_rule]->left, location, rules[current_rule]->right);	
		printf("%s\n", string);
		//printf("main string is now %s\n", string);
		return rules[current_rule]->next;
	}
	else{
		return current_rule + 1;
	}
}//execute_rule

//MAIN FUNCTION
int main(){
	struct rule* rules[MAX_RULES];
	for(int i = 0; i < MAX_RULES; i++){
		rules[i] = 0;
	}
	read_rules_from_stdin(rules);
	print_rules(rules);
	char* MainString = malloc(MAX_STRING_LEN);
	int rule_exec_count = 0;
	int next_rule = 0;
	printf("rule\tMainString (after the rule)\n");
	while((rules[next_rule] != 0)&&(rule_exec_count < MAX_RULES)){
		//printf("%d\t\t",next_rule);
		next_rule = execute_rule(rules, next_rule, MainString);
		rule_exec_count++;
	}
	if(rule_exec_count==MAX_RULES){
		printf("max # of rules reached!\n");
	}
	return 0;
}//MAIN


//the following code tests the string operations if placed in main
/*	char* s1 = "spiddlespaddlespuff";
	char* s2 = "s";
	char* s3 = "cocak";
	char* s4 = "iddle";
	char* s5 = "puff";
	char* s6 = "m";
	char* s7 = "spiddlespaddlespuff";
	char* s8 = "spaddle";
	char* s9 = "cottonpickingfingerlickingchickenplucker";
	char* s10 = "puffballs";
	printf("%d\n", search_for_substring(s2, s1));
		printf("%d\n", search_for_substring(s3, s1));
	printf("%d\n", search_for_substring(s4, s1));
	printf("%d\n", search_for_substring(s5, s1));
	printf("%d\n", search_for_substring(s6, s1));
printf("%d\n", search_for_substring(s7, s1));
printf("%d\n", search_for_substring(s8, s1));
printf("%d\n", search_for_substring(s9, s1));
printf("%d\n", search_for_substring(s10, s1));
*/
/*
	char* s11 = "sesquipedalian";
	char* s = malloc(20);
	my_strncpy(s, s11, 20);
	char* s12 = "ses";
	char* s13 = "nos";
	char* s14 = "pedal";
	char* s15 = "manul";
	char* s16 = "n";
	char* s17 = "q";
	replace_substring(s, s12, 0, s13);
	replace_substring(s, s14, 6, s15);
	replace_substring(s, s16, 13, s17);

	shift_substring_left(s, 6, 3);
	printf("After shifting: %s\n", s);

	char* s18 = "nu";
	char* s19 = "";
	char* s20 = "nos";
	char* s21 = "uf";
	replace_substring(s, s18, 5, s19);
	replace_substring(s, s20, 0, s21);

	shift_substring_right(s, 4, 3);
	printf("after shiften: %s\n",s);
	shift_substring_right(s,7,3);
	printf("after shifting: %s\n",s);
	shift_substring_right(s,4,9);
	printf("after shifting: %s\n",s);
	

	replace_substring(s, s21, 0, "stinkypete");

	replace_substring(s, "lialialia", 12, "swillachachacha");
	printf("%s\n", s);
	char* c = malloc(10);
	my_strncpy(c, "chacha", MAX_PATTERN_LEN);
	printf("cha at %d\n", search_for_substring(c, s));
	my_strncpy(c, "stink", MAX_PATTERN_LEN);
	printf("stink at %d\n", search_for_substring(c, s));
*/
