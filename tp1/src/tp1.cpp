//============================================================================
// Name        : tp1.cpp
// Author      :
// Version     :
// Description : Proceso principal ConcuParking
//============================================================================

#include <iostream>
#include <time.h>

#include "constantes.h"
#include "MemoriaCompartida.h"
#include "Semaforo.h"
#include "Estacionamiento.h"
#include "Entrada.h"
#include "Proceso.h"

using namespace std;

int main() {
    // Tiempo de inicio guardado en memoria compartida
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, 't');

    Entrada* entrada1 = new Entrada("1", 10);
    Entrada* entrada2 = new Entrada("2", 10);
    Entrada* entrada3 = new Entrada("3", 10);

    inicio.escribir(time(NULL));
    entrada1->iniciar();
    entrada2->iniciar();
    entrada3->iniciar();

    /*
	try {
	    cout << "Ejemplo de semaforos" << endl;
		Semaforo sem((char*)"tmp", 0);
		cout << "Semaforo creado" << endl;
		sem.signal();
		cout << "Semaforo señalizado, entrando en wait" << endl;
        sem.wait();
        cout << "Espera terminada" << endl;
        sem.eliminar();
        cout << "Semaforo eliminado" << endl;
	} catch (char* error) {
		cout << error;
	}
	*/
	return 0;
}
