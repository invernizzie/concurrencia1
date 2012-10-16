#include "include/Auto.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>

#include <iostream>

Auto :: Auto() {
    srand (time(NULL) + getpid());
}

void Auto :: ejecutar() {
    buscarLugar();
    sleep(determinarEspera());
    liberarLugar();
    pagar();
}

// Estrategia: comienza en posicion 0, mira una a una
// Si llega al final vuelve a empezar
void Auto :: buscarLugar() {
    cout << "Auto " << getpid() << " buscando lugar" << endl;
    int cantidad = estacionamiento.getCapacidad();
    bool encontrado = false;
    posicion = 0;
    while (!encontrado) {
        try {
            estacionamiento.ocupar(posicion);
            cout << "Auto " << getpid() << " ocupo lugar " << posicion << endl;
            encontrado = true;
        } catch (exception e) {
            posicion = (posicion + 1) % cantidad;
        }
    }
}

void Auto :: liberarLugar() {
    cout << "Auto " << getpid() << " libero lugar " << posicion << endl;
    estacionamiento.liberar(posicion);
}

unsigned Auto :: determinarEspera() {
    espera = rand() % (ESTADIA_MAXIMA - 1) + 1;
    return espera;
}

void Auto :: pagar() {
    // Esto seria el pago
    estacionamiento.registrarPago(espera * estacionamiento.getValorHora() );

    cout << "Auto " << getpid() << " se retira" << endl;
    // Esto debe hacerlo la salida
    estacionamiento.liberarLugar();
}
