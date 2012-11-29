#include "include/Proceso.h"

#include <unistd.h>

pid_t Proceso :: iniciar() {
    pid = fork();

    if (pid < 0) {
        throw errno;
    }

    if (pid == 0) {
    	pid = getpid();

    	LockFile lockCantProcesos((char*) ARCHIVO_LOCK_CANT_PROCESOS);
    	MemoriaCompartida<int> cantProcesos;
    	cantProcesos.crear((char*) ARCHIVO_AUXILIAR, C_SHM_CANT_PROCESOS);
    	lockCantProcesos.tomarLock();
    	cantProcesos.escribir(cantProcesos.leer() + 1);
    	lockCantProcesos.liberarLock();

        this->ejecutar();

        lockCantProcesos.tomarLock();
        cantProcesos.escribir(cantProcesos.leer() - 1);
        if (cantProcesos.leer() == 0) {
        	// liberar semaforo para que el main
        	// pueda destruir las colas de mensajes
        }
        cantProcesos.liberar();
        lockCantProcesos.liberarLock();

        // Es obligatorio crear el Proceso con new
        delete this;
        // Este proceso nunca abandona este scope
        exit(0);
    }
    return pid;
}


