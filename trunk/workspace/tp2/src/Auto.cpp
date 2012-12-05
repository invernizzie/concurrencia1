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

void Auto :: ejecutar() {
	inicializar();
    buscarLugar();
    sleep(determinarEspera());
    pagar();
    salir();
    deinicializar();
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

void Auto::deinicializar() {
	delete colaPedidos;
	delete colaRespuestas;

    stringstream ss;
    ss << "Auto " << getpid() << "(" << nroEstacionamiento << ") deinicializado";
	Logger::write(DEBUG, ss.str());
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

void Auto :: pagar() {

	stringstream ss;
	Pedido pedidoLiberacion;
	pedidoLiberacion.mtype = P_PAGO_DESOCUPO;
	pedidoLiberacion.pid = pid;
	pedidoLiberacion.nroEstacionamiento = nroEstacionamiento;
	pedidoLiberacion.nroLugar = posicion;
	pedidoLiberacion.duracionEstadia = espera;

	colaPedidos->escribir(pedidoLiberacion);
	ss << "Auto " << pid << "(" << nroEstacionamiento << "): Envio pago por " << espera << " horas de estadia";
		Logger::write(DEBUG, ss.str());

	Respuesta respuestaLiberacion;

	colaRespuestas->leer(pid,&respuestaLiberacion);

	ss.str("");
	if(respuestaLiberacion.respuesta.pagoAceptado){
		ss << "Auto " << pid << "(" << nroEstacionamiento << "): pudo pagar ";
		Logger::write(DEBUG, ss.str());
	}else{
		ss << "Auto " << pid << "(" << nroEstacionamiento << "):  NO pudo pagar ";
		Logger::write(DEBUG, ss.str());
	}
}

void Auto :: salir() {
	stringstream ss;
	unsigned salidaElegida = determinarSalida();
	long salida = nroEstacionamiento*1000 + salidaElegida*100 + P_SE_VA;
	Cola<Pedido> colaPedidoSalida((char*)ARCHIVO_COLAS_SALIDA, C_LOCK_COLA_PEDIDOS);
	Cola<Respuesta> colaRespuestaSalida((char*)ARCHIVO_COLAS_SALIDA, C_LOCK_COLA_RESPUESTAS);

	Pedido pedidoLiberacion;
	pedidoLiberacion.mtype = salida;
	pedidoLiberacion.pid = pid;
	pedidoLiberacion.nroEstacionamiento = nroEstacionamiento;
	pedidoLiberacion.nroLugar = posicion;
	pedidoLiberacion.duracionEstadia = espera;

	ss << "Auto " << pid << "(" << nroEstacionamiento << "): Pide libracion de lugar " << posicion << "a Salida: " << salidaElegida;
	Logger::write(DEBUG, ss.str());
	colaPedidoSalida.escribir(pedidoLiberacion);

	Respuesta respuestaLiberacion;

	colaRespuestaSalida.leer(pid,&respuestaLiberacion);

	if(respuestaLiberacion.respuesta.lugarLiberado){
		ss << "Auto " << pid << "(" << nroEstacionamiento << "): libere lugar " << posicion;
		Logger::write(DEBUG, ss.str());
	}else{
		ss << "Auto " << pid << "(" << nroEstacionamiento << "): No pudo liberar lugar " << posicion;
				Logger::write(DEBUG, ss.str());
	}
}

unsigned Auto :: determinarEspera() {
    espera = rand() % (ESTADIA_MAXIMA - 1) + 1;
    return espera;
}
