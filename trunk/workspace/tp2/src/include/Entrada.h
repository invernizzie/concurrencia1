#ifndef ENTRADA_H_
#define ENTRADA_H_

#include <string>

#include "Proceso.h"
#include "MemoriaCompartida.h"
#include "Cola.h"
#include "Mensajes.h"

using namespace std;

class Entrada : public Proceso {
    private:
		int nroEstacionamiento;
		string nombre;
		int tiempoSimulacion;
        int instanteFinal;
        Cola<Pedido>* colaPedidos;
        Cola<Respuesta>* colaRespuestas;

        void inicializar();
        void recibirAutos();
        unsigned autosPorHora();
        bool hayLugar();

    public:
        Entrada(int nroEstacionamiento, string nombre, int tiempoSimulacion);
        virtual void ejecutar();
        virtual ~Entrada() {};
};

#endif //ENTRADA_H_
