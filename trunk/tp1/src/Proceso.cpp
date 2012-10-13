#include "Proceso.h"

void Proceso :: iniciar() {
    pid_t id = fork();

    if (id < 0) {
        throw errno;
    }

    if (id == 0) {
        this->ejecutar();

        // This process will never get out of this scope
        // All deinitilization needs to be done by the parent
        exit(0);
    }
}


