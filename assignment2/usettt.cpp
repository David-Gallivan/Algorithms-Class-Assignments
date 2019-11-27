//JMJ
//
//	Ternary Tree conTraption
//	CS315, 2/13/2019
//
//	by David Gallivan

#include<iostream>
#include<string>
#include<cstdlib>
#include "ttt.h"
using namespace std;

int main(int argc, char** argv){
	int count = 1000;
	if(argc>1){
		count = atoi(argv[1]);
		if(count < 1) {
			cout << "Bad count argument" << endl;
			return 0;
		}
	}
	TTT<int> ttt;
	int number;
	for(int i=0; i < count; i++){
		cin >> number;
		ttt.insert(number);
	}
	ttt.print();
	return 0;
}
