#include "include/Administrador.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <exception>

#include "include/constantes.h"
#include "include/Auto.h"
#include "include/logger.h"

Administrador :: Administrador(int nroEstacionamiento, int tiempoSimulacion):
		nroEstacionamiento(nroEstacionamiento),
		tiempoSimulacion(tiempoSimulacion),
		instanteFinal(0),
		colaPedidos(NULL),
		colaRespuestas(NULL) {}

void Administrador :: ejecutar() {
    inicializar();
    consultarPeriodicamente();
    deinicializar();
}

void Administrador :: inicializar() {
    // Obtener tiempo de inicio de memoria compartida
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, C_SHM_TIEMPO_INICIO);
    instanteFinal = inicio.leer() + this->tiempoSimulacion;
	inicio.liberar();

	colaPedidos = new Cola<Pedido>((char*)ARCHIVO_COLAS, C_LOCK_COLA_PEDIDOS);
	colaRespuestas = new Cola<Respuesta>((char*)ARCHIVO_COLAS, C_LOCK_COLA_RESPUESTAS);

    srand (time(NULL) + getpid());
}

void Administrador::deinicializar() {
	delete colaPedidos;
	delete colaRespuestas;
}

void Administrador :: consultarPeriodicamente() {
	Respuesta r;
	Pedido pedido;
	pedido.mtype = P_CONSULTA_ESTADO;
	pedido.pid = pid;
	pedido.nroEstacionamiento = nroEstacionamiento;

	unsigned espera = tiempoEntreConsultas();
    while ((instanteFinal > time(NULL) + espera)) { // Cuando termina?
        std::stringstream ss;
        ss << "Administrador(" << nroEstacionamiento << "): esperara " << espera << " segundos";
        Logger::write(DEBUG, ss.str());

        sleep(espera);

        colaPedidos->escribir(pedido);
        colaRespuestas->leer(pid, &r);

        std::stringstream ss1;
        ss1 << "Administrador(" << nroEstacionamiento << "): hay " << r.respuesta.estado.lugaresLibres <<
        		" lugares libres, se facturo " << r.respuesta.estado.facturacion;
        Logger::write(DEBUG, ss1.str());

        espera = tiempoEntreConsultas();
    }
    stringstream ss;
    ss << "Administrador(" << nroEstacionamiento << "): finaliza la simulacion";
    Logger::write(DEBUG, ss.str());
}

unsigned Administrador :: tiempoEntreConsultas() {
    return rand() % (TIEMPO_ENTRE_CONSULTAS - 1) + 1;
}
