#include "include/Salida.h"

Salida::Salida(string nombre,int tSimulacion):nombre(nombre),tiempoSimulacion(tSimulacion) {}

Salida::~Salida()
{
    //dtor
}

void Salida::liberarEspacio(){

}

void Salida::registrarPago(){

}

void Salida::seccionCritica(){
    //this.liberarEspacio();
    //this.registrarPago();

}

void Salida::run(){

   while((difftime (time(NULL),inicio.leer()) < tiempoSimulacion) ||
   (estacionamiento.leer().getCapacidad() < estacionamiento.leer().getLugaresLibres()) )
   {
        // Tomar el primer auto qe pueda de la cola de autos_listos
            // hacer wait() de la cola de autos_listos
            // sacar el primero
            // hacer el signal

        // ejecutar el pago del ticket de ese auto
            // hacer el wait del recurso compartido CAJA
            // efectuar el pago, agregando CANT_HORAS_TICKET * VALOR_HORA al total de la caja
            // haer el signal del recurso compartido CAJA
            // marcar el ticket como PAGADO



        // liberar el espacio que ocupaba ese auto
            // hacer el wait de ese lugar
            // cambiar el estado del lugar a liberado
            // sumar 1 a espacios disponibles
            // hacer el signal del lugar

            estacionamiento.liberar();

    }

}
