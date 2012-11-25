#ifndef PROCESO_H_
#define PROCESO_H_

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "constantes.h"
#include "LockFile.h"
#include "MemoriaCompartida.h"

class Proceso {
    public:
        Proceso() {};
        virtual ~Proceso() {};
        pid_t iniciar();
        virtual void ejecutar() {};
};

#endif /* PROCESO_H_ */
