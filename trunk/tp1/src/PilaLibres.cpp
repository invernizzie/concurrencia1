using namespace std;
#include "PilaLibres.h"
#include "constantes.h"

#define REPEAT(i, x) for(int i=(x); i>0; i--)

PilaLibres :: PilaLibres(int lugares) {
    this->inicializar(lugares);
}

void PilaLibres :: inicializar(int lugares) {
    this->lugaresLibres.reserve(lugares);
    REPEAT(i, 10) {
        this->lugaresLibres.push_back(LIBRE);
    }
}
