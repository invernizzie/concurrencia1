#ifndef ESTACIONAMIENTO_H
#define ESTACIONAMIENTO_H

#include <vector>
#include <typeinfo>
#include <exception>
#include <sstream>

#include "Mensajes.h"
#include "MemoriaCompartida.h"

using namespace std;

class EstacionamientoCompleto : public exception { };

class LugarOcupado : public exception { };

class Estacionamiento {

    public:
        Estacionamiento(int capacidad, int valorHora);
        virtual ~Estacionamiento();
        bool reservarLugar();
        unsigned asignarLugarLibre();
        void liberarLugarOcupado(unsigned nroLugar);
        void cobrar(unsigned duracionEstadia);
        EstadoEstacionamiento estadoActual();

    private:
        int capacidad;
        int valorHora;
        int lugaresLibres;
        int facturacion;
        vector<unsigned> posicionesLibres;
        MemoriaCompartida < unsigned > EstacionamientosActivos;
};

#endif // ESTACIONAMIENTO_H
