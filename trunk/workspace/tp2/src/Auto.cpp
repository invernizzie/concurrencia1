#include "include/Auto.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>

#include "include/logger.h"

Auto :: Auto(int nroEstacionamiento):
		nroEstacionamiento(nroEstacionamiento),
		posicion(0),
		espera(0),
		colaPedidos(NULL),
		colaRespuestas(NULL) {}

Auto :: ~Auto() {
	delete colaPedidos;
	delete colaRespuestas;

    stringstream ss;
    ss << "Auto " << getpid() << "(" << nroEstacionamiento << "): destruido";
	Logger::write(DEBUG, ss.str());
}

void Auto :: ejecutar() {
	inicializar();
    buscarLugar();
    sleep(determinarEspera());
    liberarLugar();
	// TODO No hay cuello de botella de cantidad de salidas, introducirlo

    stringstream ss;
    ss << "Auto " << pid << "(" << nroEstacionamiento << "): se retira";
	Logger::write(DEBUG, ss.str());
}

void Auto :: inicializar() {
    srand (time(NULL) + pid);
	colaPedidos = new Cola<Pedido>((char*)ARCHIVO_COLAS, C_LOCK_COLA_PEDIDOS);
	colaRespuestas = new Cola<Respuesta>((char*)ARCHIVO_COLAS, C_LOCK_COLA_RESPUESTAS);
}

// Estrategia: comienza en posicion 0, mira una a una
// Si llega al final vuelve a empezar
void Auto :: buscarLugar() {
    stringstream ss;
    ss << "Auto " << pid << ": solicita lugar";
    Logger::write(DEBUG, ss.str());

    Pedido pedidoLugar;
    pedidoLugar.mtype = P_PIDO_LUGAR;
    pedidoLugar.pid = pid;
    pedidoLugar.nroEstacionamiento = nroEstacionamiento;

    colaPedidos->escribir(pedidoLugar);

    Respuesta respuesta;
    colaRespuestas->leer(pid, &respuesta);
    posicion = respuesta.respuesta.lugarOtorgado;

    ss.str("");
	ss << "Auto " << pid << "(" << nroEstacionamiento << "): obtuve lugar " << posicion;
	Logger::write(INFO, ss.str());
}

void Auto :: liberarLugar() {
	Pedido pedidoLiberacion;
	pedidoLiberacion.mtype = P_PAGO_Y_LIBERO;
	pedidoLiberacion.pid = pid;
	pedidoLiberacion.nroEstacionamiento = nroEstacionamiento;
	pedidoLiberacion.nroLugar = posicion;
	pedidoLiberacion.duracionEstadia = espera;

	colaPedidos->escribir(pedidoLiberacion);

	stringstream ss;
	ss << "Auto " << pid << "(" << nroEstacionamiento << "): pague y libere lugar " << posicion;
	Logger::write(DEBUG, ss.str());
}

unsigned Auto :: determinarEspera() {
    espera = rand() % (ESTADIA_MAXIMA - 1) + 1;
    return espera;
}
