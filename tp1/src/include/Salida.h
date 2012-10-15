#ifndef SALIDA_H
    #define SALIDA_H
    #include <time.h>
    #include <exception>
    #include <unistd.h>
    #include <cstring>

    #include "constantes.h"
    #include "Proceso.h"
    #include "Estacionamiento.h"
    #include "MemoriaCompartida.h"
    #include "Proceso.h"


    class Salida
    {
        private:
            string nombre;
            int tiempoSimulacion;
            MemoriaCompartida<time_t> inicio;
            MemoriaCompartida<Estacionamiento> estacionamiento;

            void seccionCritica();

            void run(); // Aca seteo la memoria compartida con los mapas de posiciones del estacionamiento

            void liberarEspacio(); // Aca seteo el espacio que libero cuando un auto deja marcada la posicion para salir

            void registrarPago(); // Aca se registra el pago luego de la liberacion del espacio que ocupaba el auto que acabo de marcar como liberada su posicion
            // unsigned tiempoEntreArribos();

        public:

            Salida(string nombre,int tSimulacion);
            virtual ~Salida();

    };

#endif // SALIDA_H
