#include "Estacionamiento.h"
#include <vector>

Estacionamiento::Estacionamiento(int capacidad) : occupied_posicions_(0), ocupacion_lock_("ocupacion_lock.tmp")
{
    shared_memories_.resize(capacidad);
    posicions_lockers_.resizq(capacidad);
    for ( int i=0 ; i < capacidad ; i++){
        create_posicion(i);
    }
}

Estacionamiento::~Estacionamiento()
{
    //dtor
}


void Estacionamiento::liberar(int posicion){
    posicions_lockers_[posicion].tomarLock();
    shared_memories_[pos_num].escribir(Parking_Status::FREE);
    posicions_lockers_[posicion].liberarLock();
}

void ocupar(int posicion){
    posicions_lockers_[posicion].tomarLock();
    Parking_Status status = shared_memories_[pos_num].leer();
    if ( status == Parking_Status::FREE ){
        shared_memories_[pos_num].escribir(Parking_Status::BUSY);
        posicions_lockers_[posicion].liberarLock();
    } else {
        posicions_lockers_[posicion].liberarLock();
        throw LugarOcupado();
    }
}

void Estacionamiento::ocuparLugar(){
    ocupacion_lock_.tomarLock();
    if ( occupied_posicions_ < shared_memories_.size() ){
        occupied_posicions_++;
        ocupacion_lock_.liberarLock();
    } else {
        ocupacion_lock_.liberarLock();
        throw EstacionamientoCompleto();
    }
}

void Estacionamiento::liberarLugar(){
    ocupacion_lock_.tomarLock();
    occupied_posicions_--;
    ocupacion_lock_.liberarLock();
}

void Estacionamiento::create_posicion(int pos_num){
    // Create the shared memory
    MemoriaCompartida<int> sh_mem;
    std::stringstream ss;
    ss << "pos_" << pos_num << ".c";
    sh_mem.crear ( ss.str().c_str() ,'R' );
    sh_mem.escribir( Parking_Status::FREE );
    shared_memories_[pos_num] = sh_mem;

    //Now create the lock for that memory
    LockFile lock(ss.str().c_str());
    posicions_lockers_[pos_num] = lock;
}
