#include "include/Salida.h"

#include <climits>

long Salida::mtypePara(int nroEstacionamiento, int nroSalida) {
	return 1000 * nroEstacionamiento + nroSalida + 1;
}

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
    comunicarCierre();

    delete colaPedidos;
    delete colaRespuestas;
    delete colaPedidoSalida;
    delete colaRespuestaSalida;

    stringstream ss;
	ss << "Salida " << numero << "(" << nroEstacionamiento << ") deinicializada";
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
	int tipoMensaje = Salida::mtypePara(nroEstacionamiento, numero);

    while (!terminar) {
    	stringstream smm;

    	colaPedidoSalida->leer(tipoMensaje, &pedidoSalida);

		switch (pedidoSalida.tipoMensaje) {

			case P_SE_VA:
				liberarLugar(pedidoSalida.pid, pedidoSalida.nroLugar, pedidoSalida.duracionEstadia);
				break;

			case P_FINALIZAR:
				smm << "Salida " << numero << "(" << nroEstacionamiento << ") finaliza la simulacion";
			    Logger::write(DEBUG, smm.str());
			    smm.str("");
				terminar = true;
				break;

			default:
				// No deberia suceder
				smm << "Salida " << numero << "(" << nroEstacionamiento << ") descarta mensaje tipo " << pedidoSalida.mtype << " de pid " << pedidoSalida.pid
					<< " el valor de mensaje es " << pedidoSalida.tipoMensaje;
				Logger::write(WARNING, smm.str());
				smm.str("");
				break;
        }
    }
}

bool Salida :: liberarLugar(pid_t _auto, unsigned posicion, unsigned espera) {
	Pedido pedido;
	pedido.mtype = P_LIBERA_LUGAR;
	pedido.pid = getpid();
	pedido.nroLugar = posicion;
	pedido.duracionEstadia = espera;
	pedido.nroEstacionamiento = nroEstacionamiento;

    stringstream ss;
	ss << "Salida " << numero << "(" << nroEstacionamiento << ") liberara lugar " << posicion << " de auto " << _auto;
	Logger::write(INFO, ss.str());

	colaPedidos->escribir(pedido);

	Respuesta respuestaDelAdmin;
	colaRespuestas->leer(pid, &respuestaDelAdmin);

	Respuesta respuestaAlAuto;
	respuestaAlAuto.mtype = _auto;
	respuestaAlAuto.respuesta.lugarLiberado = true;
	colaRespuestaSalida->escribir(respuestaAlAuto);

	ss.str("");
	ss << "Salida " << numero << "(" << nroEstacionamiento << ") libero lugar " << posicion;
	Logger::write(DEBUG, ss.str());
	return respuestaDelAdmin.respuesta.lugarLiberado;
}

void Salida::comunicarCierre() {
	Pedido pedido;
	pedido.mtype = P_TERMINO_SALIDA;
	pedido.pid = pid;
	pedido.nroEstacionamiento = nroEstacionamiento;

	colaPedidos->escribir(pedido);

    stringstream ss;
    ss << "Salida " << numero << "(" << nroEstacionamiento << ") comunica cierre al adm central";
    Logger::write(DEBUG, ss.str());
}

