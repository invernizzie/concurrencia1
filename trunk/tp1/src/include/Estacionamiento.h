#ifndef ESTACIONAMIENTO_H
#define ESTACIONAMIENTO_H
#include <vector>
#include "lugar.h"

using namespace std;

class EstacionamientoCompleto : public Exception(){ }

class LugarOcupado : public Exception() { }

class Estacionamiento {

    enum Parking_Status { FREE, BUSY };

    public:
        Estacionamiento(int capacidad = CAPACIDAD_ESTACIONAMIENTO);
        virtual ~Estacionamiento();

        // Para ser usados por el auto.
        void liberar(int posicion);
        void ocupar(int posicion);

        // Para ser usados por las entradas y las salidas.
        void liberarLugar();
        void ocuparLugar();

private:

    //std::vector< int* > parking_availability_posicions_;
    std::vector< MemoriaCompartida<int> > shared_memories_; // para lierarlas cuando ya no se usen.
    std::vector< LockFile > posicions_lockers_; //lock para cada posicion.

    int occupied_posicions_;
    LockFile ocupacion_lock_;

    void create_posicion(int pos_num);
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
