#ifndef ENTRADA_H_
#define ENTRADA_H_

#include "fork.h"

class Entrada : public Proceso {
    public:
        Entrada() {};
        virtual void ejecutar();
        virtual ~Entrada() {};
};

#endif //ENTRADA_H_
