#include "include/Salida.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <exception>
#include <string>
#include <stdlib.h>

#include "include/constantes.h"
#include "include/Auto.h"
#include "include/logger.h"

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
}

void Salida :: inicializar() {

    colaPedidos = new Cola<Pedido>((char*)ARCHIVO_COLAS, C_LOCK_COLA_PEDIDOS);
    colaRespuestas = new Cola<Respuesta>((char*)ARCHIVO_COLAS, C_LOCK_COLA_RESPUESTAS);
    colaPedidoSalida = new Cola<Pedido>((char*)ARCHIVOS_COLA_SALIDA, C_LOCK_COLA_PEDIDOS);
    colaRespuestaSalida = new Cola<Respuesta>((char*)ARCHIVOS_COLA_SALIDA, C_LOCK_COLA_RESPUESTAS);

}

void Salida :: recibirAutos() {
	bool terminar = false;
	Pedido pedidoSalida;

    while (!terminar) {
    	colaPedidoSalida->leer(-P_FINALIZAR, &pedidoSalida);
    	unsigned mensaje = pedidoSalida.mtype - (nroEstacionamiento*1000 + numero*100);

		switch (pedidoSalida.mtype) {

			case P_SE_VA:
				liberarLugar(pedidoSalida.nroLugar, pedidoSalida.duracionEstadia);
				break;

			case P_FINALIZAR:
			    stringstream ss;
			    ss << "Salida " << numero << "(" << nroEstacionamiento << "): finaliza la simulacion";
			    Logger::write(DEBUG, ss.str());
				return;

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

	colaPedidos->escribir(pedido);

	Respuesta respuesta;
	colaRespuestas->leer(pid, &respuesta);

	stringstream ss;
	ss << "Salida " << numero << "(" << nroEstacionamiento << "): recibe respuesta";
	Logger::write(INFO, ss.str());
	return respuesta.respuesta.lugarLiberado;
}

