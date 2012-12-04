#include "include/Administrador.h"

Administrador :: Administrador(int nroEstacionamiento, int tiempoSimulacion, int capacidad):
		nroEstacionamiento(nroEstacionamiento),
		tiempoSimulacion(tiempoSimulacion),
		capacidad(capacidad),
		instanteFinal(0),
		colaPedidos(NULL),
		colaRespuestas(NULL),
		colaPedidoSalida(NULL) {}

void Administrador :: ejecutar() {
    inicializar();
    consultarPeriodicamente();
    deinicializar();
    finalizarSalidas();
}

void Administrador :: inicializar() {
    // Obtener tiempo de inicio de memoria compartida
    MemoriaCompartida<time_t> inicio;

    inicio.crear((char*)ARCHIVO_AUXILIAR, C_SHM_TIEMPO_INICIO);
    instanteFinal = inicio.leer() + this->tiempoSimulacion;
	//inicio.desvincularSinBorrar();
    inicio.liberar();

	colaPedidos = new Cola<Pedido>((char*)ARCHIVO_COLAS, C_LOCK_COLA_PEDIDOS);
	colaRespuestas = new Cola<Respuesta>((char*)ARCHIVO_COLAS, C_LOCK_COLA_RESPUESTAS);

    srand (time(NULL) + getpid());
}

void Administrador::deinicializar() {
	delete colaPedidos;
	delete colaRespuestas;
    delete colaPedidoSalida;
}

void Administrador::finalizarSalidas(){
	colaPedidoSalida = new Cola<Pedido>((char*)ARCHIVOS_COLA_SALIDA, C_LOCK_COLA_PEDIDOS);
	for(int i=0; i< CANT_SALIDAS; i++)
	{
		Pedido pedido;
		pedido.mtype = nroEstacionamiento*1000 + i*100 + P_FINALIZAR;
		pedido.pid = getpid();
		pedido.nroEstacionamiento = nroEstacionamiento;

		colaPedidos->escribir(pedido);
	}
}

void Administrador :: consultarPeriodicamente() {
	bool estaVacio = false;

	Respuesta r;
	Pedido pedido;
	pedido.mtype = P_CONSULTA_ESTADO;
	pedido.pid = pid;
	pedido.nroEstacionamiento = nroEstacionamiento;

	unsigned espera = tiempoEntreConsultas();
    while (!estaVacio || (instanteFinal > time(NULL))) {
        stringstream ss;
        ss << "Administrador(" << nroEstacionamiento << "): esperara " << espera << " segundos";
        Logger::write(DEBUG, ss.str());

        sleep(espera);

        colaPedidos->escribir(pedido);
        colaRespuestas->leer(pid, &r);

        std::stringstream ss1;
        ss1 << "Administrador(" << nroEstacionamiento << "): hay " << r.respuesta.estado.lugaresLibres <<
        		" lugares libres, se facturo " << r.respuesta.estado.facturacion;
        Logger::write(DEBUG, ss1.str());

        estaVacio = r.respuesta.estado.lugaresLibres == capacidad;
        espera = tiempoEntreConsultas();
    }

    pedido.mtype = P_TERMINO_ADMINISTRADOR;
    colaPedidos->escribir(pedido);

    stringstream ss;
    ss << "Administrador(" << nroEstacionamiento << "): finaliza la simulacion";
    Logger::write(DEBUG, ss.str());
}

unsigned Administrador :: tiempoEntreConsultas() {
    return rand() % (TIEMPO_ENTRE_CONSULTAS - 1) + 1;
}
