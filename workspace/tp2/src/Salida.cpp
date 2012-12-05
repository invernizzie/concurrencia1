#include "include/Salida.h"

#include <climits>

Salida :: Salida(int nroEstacionamiento, int numero) :
	nroEstacionamiento(nroEstacionamiento),
	numero(numero),
	colaPedidos(NULL),
	colaRespuestas(NULL),
	colaPedidoSalida(NULL),
	colaRespuestaSalida(NULL) {}

void Salida :: ejecutar() {
    inicializar();
    recibirAutos();
    delete colaPedidos;
    delete colaRespuestas;
    delete colaPedidoSalida;
    delete colaRespuestaSalida;

    stringstream ss;
	ss << "Salida " << numero << "(" << nroEstacionamiento << "): deinicializada";
	Logger::write(INFO, ss.str());
}

void Salida :: inicializar() {

    colaPedidos = new Cola<Pedido>((char*)ARCHIVO_COLAS, C_LOCK_COLA_PEDIDOS);
    colaRespuestas = new Cola<Respuesta>((char*)ARCHIVO_COLAS, C_LOCK_COLA_RESPUESTAS);
    colaPedidoSalida = new Cola<Pedido>((char*)ARCHIVO_COLAS_SALIDA, C_LOCK_COLA_PEDIDOS);
    colaRespuestaSalida = new Cola<Respuesta>((char*)ARCHIVO_COLAS_SALIDA, C_LOCK_COLA_RESPUESTAS);

}

void Salida :: recibirAutos() {
	bool terminar = false;
	Pedido pedidoSalida;

	// TODO Quitar
    stringstream ss;
	ss << "Salida " << numero << "(" << nroEstacionamiento << ") leyendo mensajes hasta " << INT_MIN;
	Logger::write(INFO, ss.str());

    while (!terminar) {
    	colaPedidoSalida->leer(INT_MIN, &pedidoSalida);
    	unsigned mensaje = pedidoSalida.mtype - (nroEstacionamiento*1000 + numero*100);
    	stringstream smm;

		switch (mensaje) {

			case P_SE_VA:
				liberarLugar(pedidoSalida.nroLugar, pedidoSalida.duracionEstadia);
				break;

			case P_FINALIZAR:
			    smm << "Salida " << numero << "(" << nroEstacionamiento << "): finaliza la simulacion";
			    Logger::write(DEBUG, smm.str());
				return;
				break;

			default:
				break;
        }
    }
}

bool Salida :: liberarLugar(unsigned posicion, unsigned espera) {
	Pedido pedido;
	pedido.mtype = P_LIBERA_LUGAR;
	pedido.pid = getpid();
	pedido.nroLugar = posicion;
	pedido.duracionEstadia = espera;
	pedido.nroEstacionamiento = nroEstacionamiento;

    stringstream ss;
	ss << "Salida " << numero << "(" << nroEstacionamiento << ") liberara lugar " << posicion;
	Logger::write(INFO, ss.str());

	colaPedidos->escribir(pedido);

    ss.str("");
	ss << "Salida " << numero << "(" << nroEstacionamiento << ") esperando respuesta";
	Logger::write(INFO, ss.str());

	Respuesta respuesta;
	colaRespuestas->leer(pid, &respuesta);

	ss.str("");
	ss << "Salida " << numero << "(" << nroEstacionamiento << ") libero lugar " << posicion;
	Logger::write(INFO, ss.str());
	return respuesta.respuesta.lugarLiberado;
}

