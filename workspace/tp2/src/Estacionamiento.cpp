#include "include/Estacionamiento.h"
#include <vector>
#include <errno.h>
#include <stdlib.h>


Estacionamiento::Estacionamiento(int capacidad, int valorHora):
	capacidad(capacidad),
	valorHora(valorHora),
	lugaresLibres(capacidad),
	facturacion(0),
	entradasActivas(CANT_ENTRADAS),
	salidasActivas(CANT_SALIDAS),
	administradorActivo(true) {

	posicionesLibres.reserve(capacidad);
	for (int i=0; i < capacidad; i++) {
		posicionesLibres.push_back(i);
	}
}

Estacionamiento::~Estacionamiento() {
}

bool Estacionamiento::reservarLugar() {
	if (lugaresLibres > 0) {
		lugaresLibres--;
		return true;
	}
	return false;
}

unsigned Estacionamiento::asignarPosicionLibre() {
	unsigned posicion = posicionesLibres.back();
	posicionesLibres.pop_back();
	return posicion;
}

void Estacionamiento::liberarLugar() {
	lugaresLibres++;
}

void Estacionamiento::cobrarYLiberarPosicion(unsigned nroLugar, unsigned duracionEstadia) {
	facturacion += duracionEstadia * valorHora;
	posicionesLibres.push_back(nroLugar);
}

EstadoEstacionamiento Estacionamiento::estadoActual() {
	EstadoEstacionamiento estado;
	estado.facturacion = facturacion;
	estado.lugaresLibres = lugaresLibres;
	return estado;
}

bool Estacionamiento::estaCerrado() {
	return (!administradorActivo) && (entradasActivas <= 0) && (salidasActivas <= 0);
}

void Estacionamiento::cerrarEntrada() {
	entradasActivas--;
}

void Estacionamiento::cerrarSalida() {
	salidasActivas--;
}

void Estacionamiento::cerrarAdministrador() {
	administradorActivo = false;
}
