//============================================================================
// Name        : prueba1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

using namespace std;

int main() {
	int cant_ent = 3;
	pid_t p;
	long salto = 0;
	for (int i = 0; i< cant_ent; i++){
		salto++;
		p = fork();
		if (!p){
			break;
		}
	}
	if (!p)
	{
		cout << "Soy un proceso hijo" << endl;

		time_t start;
		time (&start);
		cout << "salto vale: " << salto << endl;
		srand ( time(NULL) + salto);
		int  espera = rand() % 10 +1;
		cout << "Voy a esperar " << espera << " segundos antes de generar un proceso" << endl;
		while (difftime (time(NULL),start) < espera);

		p = fork();
		if (!p)
		{
			cout << "Soy un proceso nieto" << endl;
			cout << "Espere: " << espera << " segundos" << endl;
		}
		else
				cout << "Soy un proceso hijo de nuevo" << endl;
	}
	else
		cout << "Soy un proceso padre" << endl;
	return 0;

}
