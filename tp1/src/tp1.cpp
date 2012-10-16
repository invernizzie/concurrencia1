//============================================================================
// Name        : tp1.cpp
// Author      :
// Version     :
// Description : Proceso principal ConcuParking
//============================================================================

#include <iostream>
#include <time.h>
#include <fstream>

#include "include/constantes.h"
#include "include/MemoriaCompartida.h"
#include "include/Semaforo.h"
#include "include/Estacionamiento.h"
#include "include/Entrada.h"
#include "include/Administrador.h"
#include "include/Proceso.h"

using namespace std;

void crearArchivosAuxiliares(int capacidadEstacionamiento);

int main() {

    // Archivos utilizados por ftok y flock
    crearArchivosAuxiliares(CAPACIDAD_ESTACIONAMIENTO);

    // Tiempo de inicio guardado en memoria compartida
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, 't');

    Estacionamiento e;
    // Si se llama en mas de un proceso, se reinicializa
    // la memoria compartida perdiendo valores ya escritos
    e.inicializarMemoria();

    Administrador* administrador = new Administrador(10);
    Entrada* entrada1 = new Entrada("1", 10);
    Entrada* entrada2 = new Entrada("2", 10);
    Entrada* entrada3 = new Entrada("3", 10);

    inicio.escribir(time(NULL));
    administrador->iniciar();
    entrada1->iniciar();
    entrada2->iniciar();
    entrada3->iniciar();

	return 0;
}

void crearArchivo(const char* nombre) {
    fstream f;
    f.open(nombre, fstream::out);
    f.close();
}

void crearArchivosAuxiliares(int capacidadEstacionamiento) {
    crearArchivo(ARCHIVO_AUXILIAR);
    crearArchivo(ARCHIVO_LOCK_OCUPACION);
    crearArchivo(ARCHIVO_LOCK_FACTURACION);
    for (int i = 0; i < capacidadEstacionamiento; i++) {
        stringstream ss;
        ss << "pos_" << i << ".c";
        crearArchivo(ss.str().c_str());
    }
}
