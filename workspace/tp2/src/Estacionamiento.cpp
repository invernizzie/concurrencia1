#include "include/Estacionamiento.h"
#include <vector>
#include <errno.h>
#include <stdlib.h>

#include "include/logger.h"

Estacionamiento::Estacionamiento(int capacidad, int valorHora):
	capacidad(capacidad),
	valorHora(valorHora),
	lugaresLibres(capacidad),
	facturacion(0) {

	posicionesLibres.reserve(capacidad);
	for (int i=0; i < capacidad; i++) {
		posicionesLibres.push_back(i);
	}
	EstacionamientosActivos.crear((char*)ARCHIVO_LOCK_CANT_ESTAC, C_SHM_CANT_ESTACIONAMIENTOS);

}

Estacionamiento::~Estacionamiento() {
	int estacActvos = EstacionamientosActivos.leer();
	EstacionamientosActivos.escribir(--estacActvos);
	EstacionamientosActivos.liberar();
}

bool Estacionamiento::reservarLugar() {
	if (lugaresLibres > 0) {
		lugaresLibres--;
		return true;
	}
	return false;
}

unsigned Estacionamiento::asignarLugarLibre() {
	unsigned posicion = posicionesLibres.back();
	posicionesLibres.pop_back();
	return posicion;
}

void Estacionamiento::liberarLugarOcupado(unsigned nroLugar) {
	posicionesLibres.push_back(nroLugar);
}

void Estacionamiento::cobrar(unsigned duracionEstadia) {
	facturacion += duracionEstadia * valorHora;
}

EstadoEstacionamiento Estacionamiento::estadoActual() {
	EstadoEstacionamiento estado;
	estado.facturacion = facturacion;
	estado.lugaresLibres = lugaresLibres;
	return estado;
}
