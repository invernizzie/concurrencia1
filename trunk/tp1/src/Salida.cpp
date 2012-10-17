#include "include/Salida.h"

Salida::Salida(string nombre,int tSimulacion):nombre(nombre),tiempoSimulacion(tSimulacion) {}

Salida::~Salida()
{
    //dtor
}

void Salida::liberarEspacio(){
    estacionamiento.liberar();
}

void Salida::run(){

   while((difftime (time(NULL),inicio.leer()) < tiempoSimulacion) ||
   (estacionamiento.leer().getCapacidad() < estacionamiento.leer().getLugaresLibres()) )
   {
        // Tomar el primer auto qe pueda de la cola de autos_listos
            // hacer wait() de la cola de autos_listos
            // sacar el primero
            // hacer el signal

            liberarEspacio();

    }

}
