#include "include/Proceso.h"

pid_t Proceso :: iniciar() {
    pid_t id = fork();

    if (id < 0) {
        throw errno;
    }

    if (id == 0) {
        this->ejecutar();

        // Es obligatorio crear el Proceso con new
        delete this;
        sleep(10);
        // Este proceso nunca abandona este scope
        exit(0);
    }
    return id;
}


