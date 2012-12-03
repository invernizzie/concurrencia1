#include "include/Estacionamiento.h"
#include <vector>
#include <errno.h>
#include <stdlib.h>

#include "include/logger.h"

Estacionamiento::Estacionamiento(int capacidad, int valorHora):
	capacidad(capacidad),
	valorHora(valorHora),
	lugaresLibres(capacidad),
	facturacion(0),
	entradasActivas(CANT_ENTRADAS),
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

unsigned Estacionamiento::asignarLugarLibre() {
	unsigned posicion = posicionesLibres.back();
	posicionesLibres.pop_back();
	return posicion;
}

void Estacionamiento::liberarLugarOcupado(unsigned nroLugar) {
	posicionesLibres.push_back(nroLugar);
	lugaresLibres++;
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

bool Estacionamiento::estaCerrado() {
	return (!administradorActivo) && (entradasActivas <= 0);
}

void Estacionamiento::cerrarEntrada() {
	entradasActivas--;
}

void Estacionamiento::cerrarAdministrador() {
	administradorActivo = false;
}
