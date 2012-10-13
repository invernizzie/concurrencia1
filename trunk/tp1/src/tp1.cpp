//============================================================================
// Name        : tp1.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
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
    // Creacion del estacionamiento en memoria compartida
    MemoriaCompartida<Estacionamiento> estacionamiento;
    estacionamiento.crear((char*)ARCHIVO_AUXILIAR, 'e');
    Estacionamiento e;
    estacionamiento.escribir(e);

    // Tiempo de inicio guardado en memoria compartida
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, 't');

    Entrada entrada(10);

    inicio.escribir(time(NULL));
    entrada.iniciar();

    estacionamiento.liberar();

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
