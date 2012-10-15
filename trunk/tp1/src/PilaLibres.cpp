using namespace std;
#include "include/PilaLibres.h"
#include "include/constantes.h"

#define REPEAT(i, x) for(int i=(x); i>0; i--)

PilaLibres :: PilaLibres(int lugares) {
    this->inicializar(lugares);
    this->semaforo = new Semaforo((char*)ARCHIVO_AUXILIAR, 0);
}

void PilaLibres :: inicializar(int lugares) {
    this->lugaresLibres.reserve(lugares);
    REPEAT(i, 10) {
        this->lugaresLibres.push_back(LIBRE);
    }
}

PilaLibres :: ~PilaLibres() {
    this->semaforo->eliminar();
    delete this->semaforo;
}
