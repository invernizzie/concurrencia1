#ifndef ESTACIONAMIENTO_H
#define ESTACIONAMIENTO_H
#include <vector>
#include "lugar.h"

using namespace std;

class Estacionamiento {

    public:
        Estacionamiento();
        virtual ~Estacionamiento();
        void liberar(int posicion);
        void ocupar(int posicion);
        int posicionesLibres();
        void ocuparPosicion();

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
