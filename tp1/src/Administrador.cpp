#include "include/Administrador.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <exception>

#include "include/constantes.h"
#include "include/Auto.h"
#include "logger.h"

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
        stringstream ss;
        ss << "Administrador esperara " << espera << " segundos";
        Logger::write(DEBUG, ss.str());

        sleep(espera);

        lugaresLibres = estacionamiento.getLugaresLibres();

        stringstream ss1;
        ss1 << "Administrador: hay " << lugaresLibres << " lugares libres, se facturo " << estacionamiento.getValorFacturado();
        Logger::write(DEBUG, ss1.str());

        espera = tiempoEntreConsultas();
    }
    stringstream ss;
    ss << "Administrador finaliza la simulacion";
    Logger::write(DEBUG, ss.str());
}

unsigned Administrador :: tiempoEntreConsultas() {
    return rand() % (TIEMPO_ENTRE_CONSULTAS - 1) + 1;
}
