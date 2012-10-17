#ifndef AUTO_H_
#define AUTO_H_

#include "Proceso.h"
#include "Estacionamiento.h"

class Auto : public Proceso {
    private:
        unsigned posicion;
        Estacionamiento estacionamiento;

        void buscarLugar();
        void liberarLugar();
        unsigned determinarEspera();
        unsigned determinarSalida(){return rand() % (CANT_SALIDAS - 1);};
        void pagar();

    public:
        unsigned espera;
        Auto();
        virtual void ejecutar();
        virtual ~Auto() {};
};

#endif // AUTO_H_
