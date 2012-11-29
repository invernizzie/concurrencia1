#ifndef AUTO_H_
#define AUTO_H_

#include "Proceso.h"
#include "Cola.h"
#include "Mensajes.h"

class Auto : public Proceso {
    private:
		int nroEstacionamiento;
        unsigned posicion;
        unsigned espera;
		Cola<Pedido>* colaPedidos;
		Cola<Respuesta>* colaRespuestas;

		void inicializar();
        void buscarLugar();
        void liberarLugar();
        unsigned determinarEspera();
        unsigned determinarSalida(){return rand() % (CANT_SALIDAS - 1);};

    public:
        Auto(int nroEstacionamiento);
        virtual void ejecutar();
        virtual ~Auto();
};

#endif // AUTO_H_
