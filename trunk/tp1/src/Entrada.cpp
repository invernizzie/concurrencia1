#include "Entrada.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <exception>

#include "constantes.h"

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
    unsigned espera = tiempoEntreArribos();
    while (instanteFinal > time(NULL) + espera) {
        cout << "Entrada " << nombre << " esperara " << espera << " segundos" << endl;
        sleep(espera);
        try {
            estacionamiento.ocuparLugar();
            // Forkear proceso Auto
        } catch (exception e) {
            // Un auto se retira
        }
        espera = tiempoEntreArribos();
    }
    cout << "Entrada " << nombre << " finaliza la simulacion" << endl;
}

unsigned Entrada :: tiempoEntreArribos() {
    return rand() % (ESTADIA_MAXIMA - 1) + 1;
}
