#include "include/AdministradorCentral.h"

#include "include/logger.h"

AdministradorCentral::AdministradorCentral(int cantEstacionamientos, int capacidad, int valorHora):
	cantEstacionamientos(cantEstacionamientos),
	capacidad(capacidad),
	valorHora(valorHora),
	estacionamiento(NULL),
	colaPedidos(NULL),
	colaRespuestas(NULL) {}

void AdministradorCentral::ejecutar() {
	EstacionamientosActivos.crear((char*)ARCHIVO_LOCK_CANT_ESTAC, C_SHM_CANT_ESTACIONAMIENTOS);
	inicializar();
	servirPedidos();
};

void AdministradorCentral::inicializar() {
	estacionamiento = new Estacionamiento*[cantEstacionamientos];
	int estacActivos;
	for (estacActivos = 0; estacActivos < cantEstacionamientos; estacActivos++) {
		estacionamiento[estacActivos] = new Estacionamiento(capacidad, valorHora);
	}
	EstacionamientosActivos.escribir(estacActivos);
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
	EstacionamientosActivos.liberar();
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
	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	e.liberarLugarOcupado(pedido.nroLugar);
	e.cobrar(pedido.duracionEstadia);

	stringstream ss;
	ss << "Administrador central libera lugar " <<
		pedido.nroLugar << " de estac " <<
		pedido.nroEstacionamiento << " de auto " << pedido.pid;
	Logger::write(INFO, ss.str());
};

void AdministradorCentral::asignarPosicion(Pedido& pedido) {
	Respuesta r;
	r.mtype = pedido.pid;

	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	r.respuesta.lugarOtorgado = e.asignarLugarLibre();

	colaRespuestas->escribir(r);

	stringstream ss;
	ss << "Administrador central asigna lugar " <<
		r.respuesta.lugarOtorgado << " de estac " <<
		pedido.nroEstacionamiento << " a auto " << pedido.pid;
	Logger::write(INFO, ss.str());
};

void AdministradorCentral::reservarLugar(Pedido& pedido) {
	Respuesta r;
	r.mtype = pedido.pid;

	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	r.respuesta.habiaLugar = e.reservarLugar();

	colaRespuestas->escribir(r);

	stringstream ss;
	ss << "Administrador central " <<
		(r.respuesta.habiaLugar ? "" : "no") <<
		" pudo reservar lugar para entrada " << pedido.pid << " de estac " << pedido.nroEstacionamiento;
	Logger::write(INFO, ss.str());
};

void AdministradorCentral::informarEstado(Pedido& pedido) {
	Respuesta r;
	r.mtype = pedido.pid;

	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	r.respuesta.estado = e.estadoActual();

	colaRespuestas->escribir(r);
	stringstream ss;
	ss << "Administrador central informa estado al adm " << pedido.pid << " de estac " << pedido.nroEstacionamiento;
	Logger::write(INFO, ss.str());
};
