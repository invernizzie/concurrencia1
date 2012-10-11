//============================================================================
// Name        : tp1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Semaforo.h"
using namespace std;

int main() {
	try {
		Semaforo sem("tmp", 0);
		sem.v();
		sem.p();
	} catch (char* error) {
		cout << error;
	}
	return 0;
}
