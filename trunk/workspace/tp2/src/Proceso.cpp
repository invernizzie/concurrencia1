#include "include/Proceso.h"

#include <unistd.h>

pid_t Proceso :: iniciar() {
    pid = fork();

    if (pid < 0) {
        throw errno;
    }

    if (pid == 0) {
    	pid = getpid();

        this->ejecutar();

        // Es obligatorio crear el Proceso con new
        delete this;
        // Este proceso nunca abandona este scope
        exit(0);
    }
    return pid;
}


