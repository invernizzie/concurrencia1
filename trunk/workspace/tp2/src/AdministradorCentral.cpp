#include "include/AdministradorCentral.h"

AdministradorCentral::AdministradorCentral(int cantEstacionamientos, int capacidad, int valorHora):
	cantEstacionamientos(cantEstacionamientos),
	capacidad(capacidad),
	valorHora(valorHora),
	estacionamiento(NULL),
	colaPedidos(NULL),
	colaRespuestas(NULL) {}

void AdministradorCentral::ejecutar() {
	inicializar();
	servirPedidos();
	deinicializar();
};

void AdministradorCentral::inicializar() {
	estacionamiento = new Estacionamiento*[cantEstacionamientos];
	int estacActivos;
	for (estacActivos = 0; estacActivos < cantEstacionamientos; estacActivos++) {
		estacionamiento[estacActivos] = new Estacionamiento(capacidad, valorHora);
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
	colaPedidos->destruir();
	colaRespuestas->destruir();
	delete colaPedidos;
	delete colaRespuestas;

    // Las siguientes solo se utilizan aqui para eliminarlas
	Cola<Pedido> colaPedidoSalida((char*)ARCHIVO_COLAS_SALIDA, C_LOCK_COLA_PEDIDOS);
	Cola<Respuesta> colaRespuestaSalida((char*)ARCHIVO_COLAS_SALIDA, C_LOCK_COLA_RESPUESTAS);
	colaPedidoSalida.destruir();
	colaRespuestaSalida.destruir();

	stringstream ss;
	ss << "Administrador central terminado";
	Logger::write(INFO, ss.str());
}

bool AdministradorCentral::hayEstacionamientosActivos() {
	bool todosCerrados = true;
	for (int i = 0; i < cantEstacionamientos; i++) {
		Estacionamiento& e = *estacionamiento[i];
		todosCerrados = todosCerrados && e.estaCerrado();
	}
	return !todosCerrados;
}

void AdministradorCentral::servirPedidos() {
	Pedido pedido;
	while (hayEstacionamientosActivos()) {
		colaPedidos->leer(-P_TERMINO_ADMINISTRADOR, &pedido);

		switch (pedido.mtype) {
			case P_PAGO_DESOCUPO:
				cobrarEstadia(pedido);
				break;

			case P_LIBERA_LUGAR:
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

			case P_TERMINO_ENTRADA:
				decrementarEntradasActivas(pedido);
				break;

			case P_TERMINO_SALIDA:
				decrementarSalidasActivas(pedido);
				break;

			case P_TERMINO_ADMINISTRADOR:
				decrementarAdministradoresActivos(pedido);
				break;

			default:
				break;
		}
	}
}

void AdministradorCentral::cobrarEstadia(Pedido& pedido) {
	Respuesta r;
	r.mtype = pedido.pid;
	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	e.cobrarYLiberarPosicion(pedido.nroLugar, pedido.duracionEstadia);

	r.respuesta.pagoAceptado = true;
	colaRespuestas->escribir(r);

	stringstream ss;
	ss << "Administrador central cobra al auto " << pedido.pid <<
			" de estac " << pedido.nroEstacionamiento;
	Logger::write(DEBUG, ss.str());
};

void AdministradorCentral::liberarPosicion(Pedido& pedido) {
	Respuesta r;
	r.mtype = pedido.pid;
	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	e.liberarLugar();
	r.respuesta.lugarLiberado = true;
	colaRespuestas->escribir(r);

	stringstream ss;
	ss << "Administrador central libera lugar " << pedido.nroLugar << " de estac "
			<< pedido.nroEstacionamiento << " por salida " << pedido.pid;
	Logger::write(DEBUG, ss.str());
};

void AdministradorCentral::asignarPosicion(Pedido& pedido) {
	Respuesta r;
	r.mtype = pedido.pid;

	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	r.respuesta.lugarOtorgado = e.asignarPosicionLibre();

	colaRespuestas->escribir(r);

	stringstream ss;
	ss << "Administrador central asigna lugar " << r.respuesta.lugarOtorgado <<
			" de estac " << pedido.nroEstacionamiento << " a auto " << pedido.pid;
	Logger::write(DEBUG, ss.str());
};

void AdministradorCentral::reservarLugar(Pedido& pedido) {
	Respuesta r;
	r.mtype = pedido.pid;

	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	r.respuesta.habiaLugar = e.reservarLugar();

	colaRespuestas->escribir(r);

	stringstream ss;
	ss << "Administrador central " << (r.respuesta.habiaLugar ? "" : "no ") <<
			"pudo reservar lugar para entrada " << pedido.pid << " de estac "
			<< pedido.nroEstacionamiento;
	Logger::write(DEBUG, ss.str());
};

void AdministradorCentral::informarEstado(Pedido& pedido) {
	Respuesta r;
	r.mtype = pedido.pid;

	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	r.respuesta.estado = e.estadoActual();

	colaRespuestas->escribir(r);
	stringstream ss;
	ss << "Administrador central informa estado al adm " << pedido.pid
			<< " de estac " << pedido.nroEstacionamiento;
	Logger::write(DEBUG, ss.str());
};

void AdministradorCentral::decrementarEntradasActivas(Pedido& pedido) {
	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	e.cerrarEntrada();

	stringstream ss;
	ss << "Administrador central cierra entrada " << pedido.pid <<
			" de estac " << pedido.nroEstacionamiento;
	Logger::write(DEBUG, ss.str());
}

void AdministradorCentral::decrementarSalidasActivas(Pedido& pedido) {
	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	e.cerrarSalida();

	stringstream ss;
	ss << "Administrador central cierra salida " << pedido.pid <<
			" de estac " << pedido.nroEstacionamiento;
	Logger::write(DEBUG, ss.str());
}

void AdministradorCentral::decrementarAdministradoresActivos(Pedido& pedido) {
	Estacionamiento& e = *estacionamiento[pedido.nroEstacionamiento];
	e.cerrarAdministrador();

	stringstream ss;
	ss << "Administrador central cierra admin  " << pedido.pid <<
			" de estac " << pedido.nroEstacionamiento;
	Logger::write(DEBUG, ss.str());
}
