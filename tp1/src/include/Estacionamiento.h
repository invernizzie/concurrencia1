#ifndef ESTACIONAMIENTO_H
#define ESTACIONAMIENTO_H

#include <vector>
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

    enum EstadoLugar { LIBRE, OCUPADO };

    public:
        Estacionamiento(int capacidad = CAPACIDAD_ESTACIONAMIENTO);
        virtual ~Estacionamiento();

        // Para ser usados por el auto.
        void liberar(int posicion);
        void ocupar(int posicion);

        // Para ser usados por las entradas y las salidas.
        void liberarLugar();
        void ocuparLugar();

        unsigned getLugaresLibres();
        unsigned getCapacidad();

    private:
        vector< MemoriaCompartida<EstadoLugar> > estadosPosicion; // para liberarlas cuando ya no se usen.
        vector< LockFile* > locksPosicion; //lock para cada posicion.

        MemoriaCompartida< unsigned > posicionesOcupadas;
        LockFile lockOcupacion;

        void crearPosicion(int);
        void destruirPosicion(int);
};

/* Clase Estacionamiento de Diego
class Estacionamiento
{
    private:
    int lugaresLibres;
    vector<estadoLugar> lugares;

    void inicializarMapa(int lugares){
        for(int i = 0; i < lugares; i++)
            this->lugares.push_back(LIBRE);
    };

    public:
        Estacionamiento(int lugares, int tiempoSimulacion){
            this->lugaresLibres = lugares;
            this->lugares.reserve(lugares);
            inicializarMapa(lugares);
        };

        Estacionamiento();
        virtual ~Estacionamiento();
    protected:
};
*/

#endif // ESTACIONAMIENTO_H
