#ifndef ENTRADA_H_
#define ENTRADA_H_

#include <vector>
#include "Semaforo.h"

/* Pila de lugares libres para estacionar.
 * Sincroniza el acceso mediante un semaforo,
 * lo cual lo convierte en un simil monitor.
 */
class PilaLibres {

    private:
        vector<int> lugaresLibres;
        Semaforo* semaforo;

        void inicializar(int);

    public:
        PilaLibres(int);
        virtual ~PilaLibres();
};

#endif //ENTRADA_H_
