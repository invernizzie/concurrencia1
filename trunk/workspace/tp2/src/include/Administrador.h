#ifndef ADMINISTRADOR_H_
#define ADMINISTRADOR_H_

#include <string>

#include "Proceso.h"
#include "Cola.h"
#include "Mensajes.h"

class Administrador : public Proceso {
    private:
		int nroEstacionamiento;
        int tiempoSimulacion;
        int instanteFinal;
        Cola<Pedido>* colaPedidos;
        Cola<Respuesta>* colaRespuestas;

        void inicializar();
        void deinicializar();
        void consultarPeriodicamente();
        unsigned tiempoEntreConsultas();

    public:
        Administrador(int nroEstacionamiento, int tiempoSimulacion);
        virtual void ejecutar();
        virtual ~Administrador() {};
};

#endif //ADMINISTRADOR_H_
