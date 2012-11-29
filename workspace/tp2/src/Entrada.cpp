#include "include/Entrada.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <exception>
#include <string>
#include <stdlib.h>

#include "include/constantes.h"
#include "include/Auto.h"
#include "include/logger.h"

Entrada :: Entrada(int nroEstacionamiento, string nombre, int tiempoSimulacion) :
			nroEstacionamiento(nroEstacionamiento),
            nombre(nombre),
            tiempoSimulacion(tiempoSimulacion),
            instanteFinal(-1),
            colaPedidos(NULL),
            colaRespuestas(NULL) {}

void Entrada :: ejecutar() {
    inicializar();
    recibirAutos();
    delete colaPedidos;
    delete colaRespuestas;
}

void Entrada :: inicializar() {
    // Obtener tiempo de inicio de memoria compartida
    MemoriaCompartida<time_t> inicio;
    inicio.crear((char*)ARCHIVO_AUXILIAR, C_SHM_TIEMPO_INICIO);
    instanteFinal = inicio.leer() + this->tiempoSimulacion;
    inicio.liberar();

    colaPedidos = new Cola<Pedido>((char*)ARCHIVO_COLAS, C_LOCK_COLA_PEDIDOS);
    colaRespuestas = new Cola<Respuesta>((char*)ARCHIVO_COLAS, C_LOCK_COLA_RESPUESTAS);

    srand (time(NULL) + getpid());
}

void Entrada :: recibirAutos() {
    while (instanteFinal > time(NULL)) {
        int autos = autosPorHora();
        stringstream ss;
        ss << "Entrada " << nombre << "(" << nroEstacionamiento << "): esta hora recibire " << autos << " autos";
        Logger::write(DEBUG, ss.str());
        for (; autos > 0; autos--) {
        	if (hayLugar()) {
        		// Si hay lugar, se forkea un Auto
				Auto* _auto = new Auto(nroEstacionamiento);
				pid_t pIdAuto = _auto->iniciar();

				stringstream ss;
				ss << "Entrada " << nombre << "(" << nroEstacionamiento << "): dejo ingresar auto " << pIdAuto;
				Logger::write(DEBUG, ss.str());

        	} else {
        		// Si no, el auto se retira
				stringstream ss;
				ss << "Entrada " << nombre << "(" << nroEstacionamiento << "): no hay lugares libres, se retira un auto";
				Logger::write(DEBUG, ss.str());
        	}
        }
        sleep(1);
    }
    stringstream ss;
    ss << "Entrada " << nombre << "(" << nroEstacionamiento << "): finaliza la simulacion";
    Logger::write(DEBUG, ss.str());
}

bool Entrada :: hayLugar() {
	Pedido pedido;
	pedido.mtype = P_OCUPO_LUGAR;
	pedido.pid = pid;
	pedido.nroEstacionamiento = nroEstacionamiento;

	colaPedidos->escribir(pedido);

	Respuesta respuesta;
	colaRespuestas->leer(pid, &respuesta);

	stringstream ss;
	ss << "Entrada " << nombre << "(" << nroEstacionamiento << "): recibe respuesta";
	Logger::write(INFO, ss.str());
	return respuesta.respuesta.habiaLugar;
}

unsigned Entrada :: autosPorHora() {
    return rand() % AUTOS_POR_HORA;
}
