#include "include/Administrador.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <exception>

#include "include/constantes.h"
#include "include/Auto.h"

#include <iostream> // TODO Quitar

Administrador :: Administrador(int tiempoSimulacion) :
            tiempoSimulacion(tiempoSimulacion) {}

void Administrador :: ejecutar() {
    inicializar();
    consultarPeriodicamente();
}

void Administrador :: inicializar() {
    // Obtener tiempo de inicio de memoria compartida
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, 't');
    instanteFinal = inicio.leer() + this->tiempoSimulacion;

    srand (time(NULL) + getpid());
}

void Administrador :: consultarPeriodicamente() {
    unsigned espera = tiempoEntreConsultas();
    while (instanteFinal > time(NULL) + espera) {
        cout << "Administrador esperara " << espera << " segundos" << endl;
        sleep(espera);

        cout << "Administrador: hay " << estacionamiento.getLugaresLibres() << " lugares libres" << endl;
        cout << "Administrador: se facturo " << estacionamiento.getValorFacturado() << endl;

        espera = tiempoEntreConsultas();
    }
    cout << "Administrador finaliza la simulacion" << endl;
}

unsigned Administrador :: tiempoEntreConsultas() {
    return rand() % (TIEMPO_ENTRE_CONSULTAS - 1) + 1;
}
