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
	ss << "Auto " << pid << "(" << nroEstacionamiento << ") envio pago por " << espera << " horas de estadia "
			<< " y libero lugar " << posicion;
		Logger::write(DEBUG, ss.str());

	Respuesta respuestaLiberacion;

	colaRespuestas->leer(pid,&respuestaLiberacion);

	ss.str("");
	if(respuestaLiberacion.respuesta.pagoAceptado){
		ss << "Auto " << pid << "(" << nroEstacionamiento << ") pude pagar ";
		Logger::write(DEBUG, ss.str());
	}else{
		// Nunca deberia suceder pero lo define el protocolo
		ss << "Auto " << pid << "(" << nroEstacionamiento << ")  NO pude pagar ";
		Logger::write(DEBUG, ss.str());
	}
}

void Auto :: salir() {
	stringstream ss;
	Cola<Pedido> colaPedidoSalida((char*)ARCHIVO_COLAS_SALIDA, C_LOCK_COLA_PEDIDOS);
	Cola<Respuesta> colaRespuestaSalida((char*)ARCHIVO_COLAS_SALIDA, C_LOCK_COLA_RESPUESTAS);

	unsigned salidaElegida = determinarSalida();
	long salida = Salida::mtypePara(nroEstacionamiento, salidaElegida);
	Pedido pedidoLiberacion;
	pedidoLiberacion.mtype = salida;
	pedidoLiberacion.tipoMensaje = P_SE_VA;
	pedidoLiberacion.pid = pid;
	pedidoLiberacion.nroEstacionamiento = nroEstacionamiento;
	pedidoLiberacion.nroLugar = posicion;
	pedidoLiberacion.duracionEstadia = espera;

	ss << "Auto " << pid << "(" << nroEstacionamiento << ") se encola en salida " << salidaElegida
			<< " con mtype " << salida;
	Logger::write(DEBUG, ss.str());
	colaPedidoSalida.escribir(pedidoLiberacion);

	Respuesta respuestaLiberacion;

	colaRespuestaSalida.leer(pid,&respuestaLiberacion);

	ss.str("");
	if(respuestaLiberacion.respuesta.lugarLiberado){
		ss << "Auto " << pid << "(" << nroEstacionamiento << ") sali";
		Logger::write(DEBUG, ss.str());
	}else{
		// Nunca deberia suceder pero lo define el protocolo
		ss << "Auto " << pid << "(" << nroEstacionamiento << ") no pude salir";
		Logger::write(DEBUG, ss.str());
	}
}

unsigned Auto :: determinarEspera() {
    espera = rand() % (ESTADIA_MAXIMA - 1) + 1;
    return espera;
}
