#include "Proceso.h"

void Proceso :: iniciar() {
    pid_t id = fork();

    if (id < 0) {
        throw errno;
    }

    if (id == 0) {
        this->ejecutar();

        // Es obligatorio crear el Proceso con new
        delete this;
        // Este proceso nunca abandona este scope
        exit(0);
    }
}


