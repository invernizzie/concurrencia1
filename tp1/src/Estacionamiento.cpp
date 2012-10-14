#include "Estacionamiento.h"
#include <vector>
#include <errno.h>

#include <iostream> // TODO Quitar

Estacionamiento::Estacionamiento(int capacidad) : lockOcupacion((char*)"ocupacion_lock.tmp")
{
    int resultado = posicionesOcupadas.crear((char*)ARCHIVO_AUXILIAR, 'p');
    if (resultado != 0) {
        cout << "Error al crear memoria compartida" << endl;
        switch (errno) {
            case EACCES:
                cout << "EACCES" << endl;
                break;
            case EBADF:
                cout << "EBADF" << endl;
                break;
            case EFAULT:
                cout << "EFAULT" << endl;
                break;
            case ELOOP:
                cout << "ELOOP" << endl;
                break;
            case ENAMETOOLONG:
                cout << "ENAMETOOLONG" << endl;
                break;
            case ENOENT:
                cout << "ENOENT" << endl;
                break;
            case ENOMEM:
                cout << "ENOMEM" << endl;
                break;
            case ENOTDIR:
                cout << "ENOTDIR" << endl;
                break;
            case EOVERFLOW:
                cout << "EOVERFLOW" << endl;
                break;
            default:
                cout << errno << endl;
        }
    }

    posicionesOcupadas.escribir((unsigned)0);

    estadosPosicion.reserve(capacidad);
    locksPosicion.reserve(capacidad);
    for ( int i=0 ; i < capacidad ; i++){
        crearPosicion(i);
    }
}

Estacionamiento::~Estacionamiento()
{
    int capacidad = estadosPosicion.size();
    for (int i = 0; i < capacidad; i++) {
        destruirPosicion(i);
    }
    posicionesOcupadas.liberar();
}


void Estacionamiento::liberar(int posicion){
    locksPosicion[posicion]->tomarLock();
    estadosPosicion[posicion].escribir(LIBRE);
    locksPosicion[posicion]->liberarLock();
}

void Estacionamiento::ocupar(int posicion){
    locksPosicion[posicion]->tomarLock();
    EstadoLugar status = estadosPosicion[posicion].leer();
    if ( status == LIBRE ){
        estadosPosicion[posicion].escribir(OCUPADO);
        locksPosicion[posicion]->liberarLock();
    } else {
        locksPosicion[posicion]->liberarLock();
        throw LugarOcupado();
    }
}

void Estacionamiento::ocuparLugar(){
    lockOcupacion.tomarLock();
    if ( posicionesOcupadas.leer() < estadosPosicion.size() ){
        posicionesOcupadas.escribir(posicionesOcupadas.leer() + 1);
        lockOcupacion.liberarLock();
    } else {
        lockOcupacion.liberarLock();
        throw EstacionamientoCompleto();
    }
}

void Estacionamiento::liberarLugar(){
    lockOcupacion.tomarLock();
    posicionesOcupadas.escribir(posicionesOcupadas.leer() - 1);
    lockOcupacion.liberarLock();
}

void Estacionamiento::crearPosicion(int pos_num){
    // Create the shared memory
    MemoriaCompartida<EstadoLugar> sh_mem;
    stringstream ss;
    ss << "pos_" << pos_num << ".c";
    sh_mem.crear ( ss.str().c_str(), 'l' );
    sh_mem.escribir( LIBRE );
    estadosPosicion.push_back(sh_mem);

    //Now create the lock for that memory
    locksPosicion.push_back( new LockFile(ss.str().c_str()) );
}

void Estacionamiento::destruirPosicion(int pos) {
    estadosPosicion[pos].liberar();
    delete locksPosicion[pos];
}
