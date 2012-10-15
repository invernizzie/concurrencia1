#ifndef ENTRADA_H_
#define ENTRADA_H_

#include <string>

#include "Proceso.h"
#include "MemoriaCompartida.h"
#include "Estacionamiento.h"

class Entrada : public Proceso {
    private:
        Estacionamiento estacionamiento;
        int tiempoSimulacion;
        int instanteFinal;
        string nombre;

        void inicializar();
        void recibirAutos();
        unsigned tiempoEntreArribos();

    public:
        Entrada(string nombre, int tiempoSimulacion);
        virtual void ejecutar();
        virtual ~Entrada() {};
};

#endif //ENTRADA_H_
