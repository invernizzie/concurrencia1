#ifndef ENTRADA_H_
#define ENTRADA_H_

#include <time.h>
#include <exception>
#include <unistd.h>

#include "constantes.h"
#include "Proceso.h"
#include "MemoriaCompartida.h"
#include "Estacionamiento.h"

class Entrada : public Proceso {
    private:
        MemoriaCompartida<Estacionamiento> estacionamiento;
        int tiempoSimulacion;
        int instanteFinal;

        void inicializar();
        void recibirAutos();
        unsigned tiempoEntreArribos();;

    public:
        Entrada(int tiempoSimulacion) : tiempoSimulacion(tiempoSimulacion) {};
        virtual void ejecutar();
        virtual ~Entrada() {};
};

#endif //ENTRADA_H_
