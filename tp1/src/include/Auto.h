#ifndef AUTO_H_
#define AUTO_H_

#include "Proceso.h"
#include "Estacionamiento.h"

class Auto : public Proceso {
    private:
        static unsigned idCount;

        unsigned id;
        unsigned posicion;
        Estacionamiento estacionamiento;

        void buscarLugar();
        void liberarLugar();
        unsigned determinarEspera();
        void pagar();

    public:
        Auto();
        virtual void ejecutar();
        virtual ~Auto() {};
};

#endif // AUTO_H_
