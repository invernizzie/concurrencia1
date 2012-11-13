//============================================================================
// Name        : tp1.cpp
// Author      :
// Version     :
// Description : Proceso principal ConcuParking
//============================================================================

#include <iostream>
#include <time.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <climits>
#include <sys/stat.h>
#include <sys/types.h>

#include "constantes.h"
#include "MemoriaCompartida.h"
#include "Estacionamiento.h"
#include "Entrada.h"
#include "Administrador.h"
#include "Proceso.h"

/* Opciones disponibles:
 * - Tiempo de simulacion (obligatorio)
 * - Capacidad
 * - Precio por hora
 * - modo Debug (archivo de salida opcional)
 */
#define OPTIONS     "t:c:p:d::"

using namespace std;

void crearArchivosAuxiliares(int capacidadEstacionamiento);

enum STR2INT_ERROR { SUCCESS, OVERFLOW, UNDERFLOW, INCONVERTIBLE };
STR2INT_ERROR str2int (int &i, char const *s, int base = 0);

int main(int argc, char **argv) {

    // Opciones con sus valores por defecto
    bool debug=false;
    string archivoLog(ARCHIVO_LOG);
    int tiempoSimulacion = TIEMPO_SIMULACION;
    int capacidad = CAPACIDAD_ESTACIONAMIENTO;
    int precio = PRECIO_SUGERIDO;

    // Evitamos mensajes de error de getopt
    opterr = 0;
    // Parseo de opciones
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            case 't':
                if ((str2int(tiempoSimulacion, optarg) != SUCCESS) || (tiempoSimulacion < 1)) {
                    cout << "La opcion -t espera una duracion entera y positiva" << endl;
                    abort();
                }
                break;

            case 'c':
                if ((str2int(capacidad, optarg) != SUCCESS) || (capacidad < 1)) {
                    cout << "La opcion -c espera una capacidad entera y positiva" << endl;
                    abort();
                }
                break;

            case 'p':
                if ((str2int(precio, optarg) != SUCCESS) || (precio < 1)) {
                    cout << "La opcion -p espera un precio entero y positivo" << endl;
                    abort();
                }
                break;

            case 'd':
                debug = true;
                if (optarg != NULL) {
                    archivoLog = optarg;
                }
                break;

            case '?':
                if ((optopt == 't') || (optopt == 'c') || (optopt == 'p')) {
                    cout << "La opcion -" << (char) optopt << " requiere un argumento" << endl;
                    abort();
                } else {
                    cout << "Atencion: la opcion -" << (char) optopt << " es desconocida y sera ignorada" << endl;
                }
                break;
            default:
                abort();
        }
    }

    // Archivos utilizados por ftok y flock
    crearArchivosAuxiliares(capacidad);

    // Tiempo de inicio, capacidad y precio guardados en memoria compartida
    MemoriaCompartida<int> memCapacidad;
    memCapacidad.crear((char*)ARCHIVO_AUXILIAR, C_SHM_CAPACIDAD);
    memCapacidad.escribir(capacidad);
    MemoriaCompartida<int> memPrecio;
    memPrecio.crear((char*)ARCHIVO_AUXILIAR, C_SHM_PRECIO);
    memPrecio.escribir(precio);
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, C_SHM_TIEMPO_INICIO);

    Estacionamiento e;
    // Si se llama en mas de un proceso, se reinicializa
    // la memoria compartida perdiendo valores ya escritos
    e.inicializarMemoria();

    Administrador* administrador = new Administrador(tiempoSimulacion);
    Entrada* entrada1 = new Entrada("1", tiempoSimulacion);
    Entrada* entrada2 = new Entrada("2", tiempoSimulacion);
    Entrada* entrada3 = new Entrada("3", tiempoSimulacion);

    inicio.escribir(time(NULL));
    administrador->iniciar();
    entrada1->iniciar();
    entrada2->iniciar();
    entrada3->iniciar();

    inicio.liberar();
    memCapacidad.liberar();
    memPrecio.liberar();

	return 0;
}

void crearArchivo(const char* nombre) {
    fstream f;
    f.open(nombre, fstream::out);
    f.close();
}

void crearArchivosAuxiliares(int capacidadEstacionamiento) {
    mkdir(DIR_AUXILIAR, 0777);
    crearArchivo(ARCHIVO_AUXILIAR);
    crearArchivo(ARCHIVO_LOCK_OCUPACION);
    crearArchivo(ARCHIVO_LOCK_FACTURACION);
    for (int i = 0; i < capacidadEstacionamiento; i++) {
        stringstream ss;
        ss << DIR_AUXILIAR << "/pos_" << i << ".c";
        crearArchivo(ss.str().c_str());
    }
}

STR2INT_ERROR str2int (int &i, char const *s, int base)
{
    char *end;
    long  l;
    errno = 0;
    l = strtol(s, &end, base);
    if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
        return OVERFLOW;
    }
    if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
        return UNDERFLOW;
    }
    if (*s == '\0' || *end != '\0') {
        return INCONVERTIBLE;
    }
    i = l;
    return SUCCESS;
}
