#ifndef ADMINISTRADOR_H_
#define ADMINISTRADOR_H_

#include <string>

#include "Proceso.h"
#include "MemoriaCompartida.h"
#include "Estacionamiento.h"

class Administrador : public Proceso {
    private:
        Estacionamiento estacionamiento;
        int tiempoSimulacion;
        int instanteFinal;

        void inicializar();
        void consultarPeriodicamente();
        unsigned tiempoEntreConsultas();

    public:
        Administrador(int tiempoSimulacion);
        virtual void ejecutar();
        virtual ~Administrador() {};
};

#endif //ADMINISTRADOR_H_
