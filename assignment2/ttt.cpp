//JMJ

/*             TTT
 *   Ternary Tree conTraption
 * TTT class template implementation
 *
 * by David Gallivan
 *
 */

#ifndef TTT_CPP
#define TTT_CPP

#include "ttt.h"

using namespace std;

/*  CONSTRUCTORS & THEIR HELPFUL FRIENDS  */
//default constructor
template<class T>
TTT<T>::TTT(){
	left = middle = right = NULL;
	setEMPTY();
	loVal = hiVal = EMPTY;
}//default constructor

//constructor with one value to insert
template<class T>
TTT<T>::TTT(T val){
	left = middle = right = NULL;
	setEMPTY();
	hiVal = EMPTY;
	loVal = val;	
}//constructor with one value

//setEMPTY - auxiliary function called by constructors
//sets the value of EMPTY based on the type of T
//currently works for int, but could be expanded
//to work for any numerical data type.
template<class T>
void TTT<T>::setEMPTY(){
	this->EMPTY = INT32_MIN;
}
/*  END CONSTRUCTORS & FRIENDS */

/*  GET FUNCTIONS  */
//high value
template<class T>
T TTT<T>::getLo(){
	return this->loVal;
}//getHi

//low value
template<class T>
T TTT<T>::getHi(){
	return this->hiVal;
}//getLo
/*  END GET FUNCTIONS  */

/*  STATUS FUNCTIONS  */
//boolean isEmpty
template<class T>
bool TTT<T>::isEmpty(){
	if(this->loVal == this->hiVal && this->hiVal == this->EMPTY)
		return true;
	else
		return false;
}//isEmpty

//boolean isFull - i.e., this TTT has both hi and lo values filled in
template<class T>
bool TTT<T>::isFull(){
	if(this->loVal != this->EMPTY && this->hiVal != this->EMPTY)
		return true;
	else
		return false;
}//isFull
/*  END STATUS FUNCTIONS  */

/*  OTHER FUNCTIONS  */
//insert - place a new value into the TTT, potentially recursively
template<class T>
void TTT<T>::insert(T newVal){  //recursive
	if(this->isEmpty())
		this->loVal = newVal;
	else if(!this->isEmpty() && !this->isFull()){
	//i.e. exactly one slot is filled, and we know it to be loVal
		if(newVal < this->loVal)
			{this->hiVal = this->loVal;
			 this->loVal = newVal;}
		else
			{this->hiVal = newVal;}
	}
	else{
	//i.e. the current node has no space
		if(newVal <= this->loVal){
			if(this->left == NULL)
				{this->left = new TTT<T>; this->left->insert(newVal);}
			else
				left->insert(newVal);
		}
		else if(newVal <= this->hiVal){
			if(this->middle == NULL)
				{this->middle = new TTT<T>; this->middle->insert(newVal);}
			else
				middle->insert(newVal);
		}
		else{
			if(this->right == NULL)
				{this->right = new TTT<T>; this->right->insert(newVal);}
			else
				right->insert(newVal);
}}}//insert

//print - calls subPrint and starts a new line
template<class T>
void TTT<T>::print(){
	subPrint();
	cout << endl;
}//print

//subPrint - recursively prints the TTT
template<class T>
void TTT<T>::subPrint(){  //recursive
		if(this->left != NULL)
			{cout << "("; this->left->subPrint(); cout << ") ";}
		cout << this->loVal;
		if(this->middle != NULL)
			{cout << " ("; this->middle->subPrint(); cout << ")";}
		if(this->isFull())
			cout << " " << this->hiVal;
		if(this->right != NULL)
			{cout << " ("; this->right->subPrint(); cout << ")";}
}//subPrint

#endif
