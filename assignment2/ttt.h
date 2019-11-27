//JMJ

/*             TTT
 *   Ternary Tree conTraption
 * TTT class template header file
 *
 * by David Gallivan
 *
 */

#ifndef TTT_H
#define TTT_H

#include<iostream>
#include<string>
#include<typeinfo>
#include<cmath>

using namespace std;

template<class T>  //Should work correctly for int, imperfectly for other numerical types
class TTT
{
	T EMPTY;
	T loVal;
	T hiVal;
	TTT<T> *left, *middle, *right;  //pointers to children
	string INT = "i", FLOAT = "f";

	public:
		TTT<T>();
		TTT<T>(T val);

		T getLo();
		T getHi();  //return the low and high values

		bool isEmpty();
		bool isFull();

		void insert(T newVal);

		void print();

	private:
		void subPrint();
		void setEMPTY();  //needs to be updated to work for float, long, etc.
};

#include"ttt.cpp"
#endif
