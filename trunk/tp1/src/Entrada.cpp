#include "Entrada.h"

void Entrada :: ejecutar() {
    this->inicializar();

    this->recibirAutos();

    this->estacionamiento.liberar();
}

void Entrada :: inicializar() {
    // Obtener estacionamiento de memoria compartida
    this->estacionamiento.crear((char*)ARCHIVO_AUXILIAR, 'e');

    // Obtener tiempo de inicio de memoria compartida
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, 't');
    this->instanteFinal = inicio.leer() + this->tiempoSimulacion;
}

void Entrada :: recibirAutos() {
    while (this->instanteFinal > time(NULL)) {
        sleep(this->tiempoEntreArribos());
        try {
            this->estacionamiento.leer().ocuparPosicion();
            // Forkear proceso Auto
        } catch (exception e) {
            // Un auto se retira
        }
    }
}

// TODO
unsigned Entrada :: tiempoEntreArribos() {
    return 1;
}
