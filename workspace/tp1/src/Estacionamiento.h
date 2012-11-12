#ifndef ESTACIONAMIENTO_H
#define ESTACIONAMIENTO_H

#include <vector>
#include <iostream>
#include <typeinfo>
#include <exception>
#include <sstream>

#include "constantes.h"
#include "LockFile.h"
#include "MemoriaCompartida.h"
//#include "lugar.h"

using namespace std;

class EstacionamientoCompleto : public exception { };

class LugarOcupado : public exception { };

class Estacionamiento {

    public:
        Estacionamiento();
        virtual ~Estacionamiento();
        void inicializarMemoria();


        // Para ser usados por el auto.
        void liberar(int posicion);
        void ocupar(int posicion);

        // Para ser usados por las entradas y las salidas.
        void liberarLugar();
        void ocuparLugar();

        unsigned getLugaresLibres();
        unsigned getCapacidad();

        float getValorFacturado();

        void registrarPago(float valor);

        float getValorHora(){return valorHora;};

        void setValorHora(float precio){valorHora = precio;};

        void salir(unsigned salida);



    private:
        vector< MemoriaCompartida<EstadoLugar> > estadosPosicion; // para liberarlas cuando ya no se usen.
        vector< LockFile* > locksPosicion; //lock para cada posicion.

        MemoriaCompartida< unsigned > posicionesOcupadas;
        LockFile lockOcupacion;

        vector< LockFile* > locksSalidas;

        MemoriaCompartida<float> valorFacturado;
        LockFile lockFacturacion;

        int valorHora;

        void crearPosicion(int);
        void destruirPosicion(int);
};

#endif // ESTACIONAMIENTO_H
