#include "Estacionamiento.h"
#include <vector>
#include <errno.h>
#include <stdlib.h>

#include <iostream> // TODO Quitar

Estacionamiento::Estacionamiento() :
    lockOcupacion((char*)ARCHIVO_LOCK_OCUPACION),
    lockFacturacion((char*)ARCHIVO_LOCK_FACTURACION) {

    // Capacidad y precio leidos de memoria compartida
    MemoriaCompartida<int> memCapacidad;
    memCapacidad.crear((char*)ARCHIVO_AUXILIAR, C_SHM_CAPACIDAD);
    int capacidad = memCapacidad.leer();
    memCapacidad.liberar();

    MemoriaCompartida<int> memPrecio;
    memPrecio.crear((char*)ARCHIVO_AUXILIAR, C_SHM_PRECIO);
    valorHora = memPrecio.leer();
    memPrecio.liberar();

    int resultado = posicionesOcupadas.crear((char*)ARCHIVO_AUXILIAR, C_SHM_POS_OCUPADAS);
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
    valorFacturado.crear((char*)ARCHIVO_AUXILIAR, C_SHM_VALOR_FACTURADO);
    estadosPosicion.reserve(capacidad);
    locksPosicion.reserve(capacidad);
    locksSalidas.reserve(CANT_SALIDAS);
    for ( int i=0 ; i < capacidad ; i++){
        crearPosicion(i);
    }
}

void Estacionamiento :: inicializarMemoria() {
    valorFacturado.escribir(0.0);
    posicionesOcupadas.escribir((unsigned)0);
    for ( unsigned i=0 ; i < getCapacidad(); i++){
        estadosPosicion[i].escribir( LIBRE );
    }
}

Estacionamiento::~Estacionamiento() {
    int capacidad = estadosPosicion.size();
    for (int i = 0; i < capacidad; i++) {
        destruirPosicion(i);
    }
    posicionesOcupadas.liberar();
}

void Estacionamiento::salir(unsigned salida){
    locksSalidas[salida]->tomarLock();
    liberarLugar();
    locksSalidas[salida]->liberarLock();
}

void Estacionamiento::liberar(int posicion){
    locksPosicion[posicion]->tomarLock();
    estadosPosicion[posicion].escribir(LIBRE);
    locksPosicion[posicion]->liberarLock();
}

void Estacionamiento::ocupar(int posicion){
    locksPosicion[posicion]->tomarLock();
    EstadoLugar estado = estadosPosicion[posicion].leer();
    if ( estado == LIBRE ){
        estadosPosicion[posicion].escribir(OCUPADO);
        locksPosicion[posicion]->liberarLock();
    } else {
        locksPosicion[posicion]->liberarLock();
        throw LugarOcupado();
    }
}

void Estacionamiento::ocuparLugar(){
    lockOcupacion.tomarLock();
    if ( posicionesOcupadas.leer() < getCapacidad() ){
        posicionesOcupadas.escribir(posicionesOcupadas.leer() + 1);
        cout << "Estacionamiento: ocupando lugar, hay " << posicionesOcupadas.leer() << " ocupadas" << endl;
        lockOcupacion.liberarLock();
    } else {
        cout << "Estacionamiento: ya no hay lugar, hay " << posicionesOcupadas.leer() << " ocupadas" << endl;
        lockOcupacion.liberarLock();
        throw EstacionamientoCompleto();
    }
}


void Estacionamiento::liberarLugar(){
    lockOcupacion.tomarLock();
    posicionesOcupadas.escribir(posicionesOcupadas.leer() - 1);
    cout << "Estacionamiento: liberando lugar, hay " << posicionesOcupadas.leer() << " ocupadas" << endl;
    lockOcupacion.liberarLock();
}

unsigned Estacionamiento::getLugaresLibres() {
    return getCapacidad() - posicionesOcupadas.leer();
}

unsigned Estacionamiento::getCapacidad() {
    return estadosPosicion.size();
}

float Estacionamiento::getValorFacturado(){
    return valorFacturado.leer();
}

void Estacionamiento::registrarPago(float pago){
    lockFacturacion.tomarLock();
    valorFacturado.escribir(valorFacturado.leer() + pago);
    lockFacturacion.liberarLock();
}

void Estacionamiento::crearPosicion(int pos_num){
    // Create the shared memory
    MemoriaCompartida<EstadoLugar> sh_mem;
    stringstream ss;
    ss << DIR_AUXILIAR << "/pos_" << pos_num << ".c";
    int resultado = sh_mem.crear ( ss.str().c_str(), C_SHM_LUGAR_ESTACIONAMIENTO );
    if (resultado != SHM_OK) {
        cout << "Error al crear memoria compartida para posicion " << pos_num << endl;
        exit(-1);
    }
    estadosPosicion.push_back(sh_mem);

    //Now create the lock for that memory
    locksPosicion.push_back( new LockFile(ss.str().c_str()) );
}

void Estacionamiento::destruirPosicion(int pos) {
    estadosPosicion[pos].liberar();
    delete locksPosicion[pos];
}
