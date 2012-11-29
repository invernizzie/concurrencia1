#include "include/AdministradorCentral.h"

#include "include/logger.h"

AdministradorCentral::AdministradorCentral(int cantEstacionamientos, int capacidad):
	cantEstacionamientos(cantEstacionamientos),
	capacidad(capacidad),
	estacionamiento(NULL),
	colaPedidos(NULL),
	colaRespuestas(NULL) {}

void AdministradorCentral::ejecutar() {
	inicializar();
	servirPedidos();
};

void AdministradorCentral::inicializar() {
	estacionamiento = new Estacionamiento*[cantEstacionamientos];
	for (int i = 0; i < cantEstacionamientos; i++) {
		estacionamiento[i] = new Estacionamiento(capacidad);
	}
    colaPedidos = new Cola<Pedido>((char*)ARCHIVO_COLAS, C_LOCK_COLA_PEDIDOS);
    colaRespuestas = new Cola<Respuesta>((char*)ARCHIVO_COLAS, C_LOCK_COLA_RESPUESTAS);

    stringstream ss;
	ss << "Administrador central inicializado";
	Logger::write(INFO, ss.str());
}

void AdministradorCentral::deinicializar() {
	for (int i = 0; i < cantEstacionamientos; i++) {
		delete estacionamiento[i];
	}
	delete[] estacionamiento;
	delete colaPedidos;
	delete colaRespuestas;

	stringstream ss;
	ss << "Administrador central terminado";
	Logger::write(INFO, ss.str());
}

void AdministradorCentral::servirPedidos() {
	Pedido pedido;
	while (true) { // TODO Cuando para?
		colaPedidos->leer(-P_CONSULTA_ESTADO, &pedido);

		stringstream ss;
		ss << "Administrador central recibe pedido tipo " << pedido.mtype << " de " << pedido.pid;
		Logger::write(INFO, ss.str());

		switch (pedido.mtype) {
			case P_PAGO_Y_LIBERO:
				liberarPosicion(pedido);
				break;

			case P_PIDO_LUGAR:
				asignarPosicion(pedido);
				break;

			case P_OCUPO_LUGAR:
				reservarLugar(pedido);
				break;

			case P_CONSULTA_ESTADO:
				informarEstado(pedido);
				break;

			default:
				break;
		}
	}
}

void AdministradorCentral::liberarPosicion(Pedido& pedido) {
	// TODO
};

void AdministradorCentral::asignarPosicion(Pedido& pedido) {
	// TODO
	Respuesta respuesta;
	respuesta.mtype = pedido.pid;
	respuesta.respuesta.lugarOtorgado = 0;

	colaRespuestas->escribir(respuesta);
};

void AdministradorCentral::reservarLugar(Pedido& pedido) {
	// TODO
	static bool hay = true;
	Respuesta respuesta;
	respuesta.mtype = pedido.pid;
	respuesta.respuesta.habiaLugar = hay;

	colaRespuestas->escribir(respuesta);

	stringstream ss;
	ss << "Administrador central reserva lugar para " << pedido.pid << ": " << (hay ? " verdadero" : " falso");
	Logger::write(INFO, ss.str());
	hay = false;
};

void AdministradorCentral::informarEstado(Pedido& pedido) {
	// TODO
};
