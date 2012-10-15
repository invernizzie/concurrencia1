#include "include/Entrada.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <exception>

#include "include/constantes.h"
#include "include/Auto.h"

#include <iostream> // TODO Quitar

Entrada :: Entrada(string nombre, int tiempoSimulacion) :
            tiempoSimulacion(tiempoSimulacion),
            nombre(nombre) {}

void Entrada :: ejecutar() {
    // Esto solamente lo hace el auto, entonces porque va a recibir autos????
    inicializar();
    // Esto no debiera no utilizarse aca??????
    recibirAutos();
}

void Entrada :: inicializar() {
    // Obtener tiempo de inicio de memoria compartida
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, 't');
    instanteFinal = inicio.leer() + this->tiempoSimulacion;

    srand (time(NULL) + getpid());
}

void Entrada :: recibirAutos() {
    unsigned espera = tiempoEntreArribos();
    while (instanteFinal > time(NULL) + espera) {
        cout << "Entrada    " << nombre << " esperara " << espera << " segundos" << endl;
        sleep(espera);
        try {
            estacionamiento.ocuparLugar();
            // Si hay lugar, se forkea un Auto
            Auto* _auto = new Auto();
            _auto->iniciar();
            cout << "Entrada " << nombre << ": quedan " <<
                estacionamiento.getLugaresLibres() << " lugares libres" << endl;
            //iniciar();
        } catch (exception e) {
            // Si no, el auto se retira
        }
        espera = tiempoEntreArribos();
    }
    cout << "Entrada " << nombre << " finaliza la simulacion" << endl;
}

unsigned Entrada :: tiempoEntreArribos() {
    return rand() % (TIEMPO_ENTRE_ARRIBOS - 1) + 1;
}
