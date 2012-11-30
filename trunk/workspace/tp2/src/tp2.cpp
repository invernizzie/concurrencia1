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

#include "include/constantes.h"
#include "include/MemoriaCompartida.h"
#include "include/AdministradorCentral.h"
#include "include/Entrada.h"
#include "include/Administrador.h"
#include "include/Proceso.h"
#include "include/logger.h"

/* Opciones disponibles:
 * - Tiempo de simulacion (obligatorio)
 * - Capacidad
 * - Precio por hora
 * - modo Debug (archivo de salida opcional)
 */
#define OPTIONS     "e:t:c:p:d::"

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
    int cantEstacionamientos = CANTIDAD_ESTACIONAMIENTOS;

    // Evitamos mensajes de error de getopt
    opterr = 0;
    // Parseo de opciones
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        	case 'e':
				if ((str2int(cantEstacionamientos, optarg) != SUCCESS) || (cantEstacionamientos < 1)) {
					cout << "La opcion -e espera una cantidad entera y positiva" << endl;
					abort();
				}
				break;

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
                if (optopt != 'd') {
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

    TipoLog modoDebug = ERROR;
    if (debug) {
        modoDebug = INFO;
    }
    Logger::initialize(modoDebug, archivoLog);

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

	AdministradorCentral admin(cantEstacionamientos, capacidad, precio);
    Administrador* admins[cantEstacionamientos];
    Entrada* entradas[cantEstacionamientos][CANT_ENTRADAS];

    // Encapsular esto en un estacionamiento?
    for (int est = 0; est < cantEstacionamientos; est++) {
    	admins[est] = new Administrador(est, tiempoSimulacion);
    	stringstream nombre;
    	for (int entrada = 0; entrada < CANT_ENTRADAS; entrada++) {
    		nombre.str("");
    		nombre << entrada;
    		entradas[est][entrada] = new Entrada(est, nombre.str(), tiempoSimulacion);
    	}
    }

    // TODO Iniciar de una manera mas seria para mejorar simultaneidad (semaforo?)
    inicio.escribir(time(NULL));
    admin.iniciar();
    for (int est = 0; est < cantEstacionamientos; est++) {
		admins[est]->iniciar();
		for (int entrada = 0; entrada < CANT_ENTRADAS; entrada++) {
			entradas[est][entrada]->iniciar();
		}
	}

    inicio.liberar();
    memCapacidad.liberar();
    memPrecio.liberar();

    for (int est = 0; est < cantEstacionamientos; est++) {
		delete admins[est];
		for (int entrada = 0; entrada < CANT_ENTRADAS; entrada++) {
			delete entradas[est][entrada];
		}
	}

    // TODO Esperar semaforo/lock de finalizacion y eliminar colas de mensajes?
    // No es mejor hacerlo cuando termina el administrador central?
    // Como determina el administrador central que tiene que terminar?

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
    crearArchivo(ARCHIVO_LOCK_CANT_PROCESOS);
    crearArchivo(ARCHIVO_COLAS);
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
