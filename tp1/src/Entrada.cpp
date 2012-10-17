#include "include/Entrada.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <exception>

#include "include/constantes.h"
#include "include/Auto.h"

#include <iostream> // TODO Quitar

Entrada :: Entrada(string nombre, int tiempoSimulacion) :
            tiempoSimulacion(tiempoSimulacion),
            nombre(nombre) {}

void Entrada :: ejecutar() {
    inicializar();
    recibirAutos();
}

void Entrada :: inicializar() {
    // Obtener tiempo de inicio de memoria compartida
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, 't');
    instanteFinal = inicio.leer() + this->tiempoSimulacion;

    srand (time(NULL) + getpid());
}

void Entrada :: recibirAutos() {
    while (instanteFinal > time(NULL)) {
        int autos = autosPorHora();
        cout << "Entrada " << nombre << ": intentando recibir " << autos << " autos" << endl;
        for (; autos > 0; autos--) {
            try {
                estacionamiento.ocuparLugar();
                // Si hay lugar, se forkea un Auto

                Auto* _auto = new Auto();
                pid_t pIdAuto = _auto->iniciar();
                cout << "Entrada " << nombre << ": dejo ingresar auto " << pIdAuto << endl;

            } catch (exception e) {
                // Si no, el auto se retira
                cout << "Entrada " << nombre << ": no hay lugares libres, se retira un auto" << endl;
            }
        }
        sleep(1);
    }
    cout << "Entrada " << nombre << " finaliza la simulacion" << endl;
}

unsigned Entrada :: autosPorHora() {
    return rand() % AUTOS_POR_HORA;
}
