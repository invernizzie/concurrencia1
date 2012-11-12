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
    inicio.crear((char*)ARCHIVO_AUXILIAR, C_SHM_TIEMPO_INICIO);
    instanteFinal = inicio.leer() + this->tiempoSimulacion;

    srand (time(NULL) + getpid());
}

void Administrador :: consultarPeriodicamente() {
    unsigned espera = tiempoEntreConsultas();
    unsigned capacidad;
    unsigned lugaresLibres = capacidad = estacionamiento.getCapacidad();
    while ((instanteFinal > time(NULL) + espera) || (lugaresLibres < capacidad)) {
        cout << "Administrador esperara " << espera << " segundos" << endl;
        sleep(espera);

        lugaresLibres = estacionamiento.getLugaresLibres();
        cout << "Administrador: hay " << lugaresLibres << " lugares libres" << endl;
        cout << "Administrador: se facturo " << estacionamiento.getValorFacturado() << endl;

        espera = tiempoEntreConsultas();
    }
    cout << "Administrador finaliza la simulacion" << endl;
}

unsigned Administrador :: tiempoEntreConsultas() {
    return rand() % (TIEMPO_ENTRE_CONSULTAS - 1) + 1;
}
