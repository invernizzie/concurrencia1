#include "include/Entrada.h"

#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <exception>
#include <string>
#include <stdlib.h>
#include <sys/wait.h>

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
    esperarAutos();
    comunicarCierre();
    deinicializar();
}

void Entrada :: inicializar() {
    // Obtener tiempo de inicio de memoria compartida
    inicio.crear((char*)ARCHIVO_AUXILIAR, C_SHM_TIEMPO_INICIO);
    instanteFinal = inicio.leer() + tiempoSimulacion;

    colaPedidos = new Cola<Pedido>((char*)ARCHIVO_COLAS, C_LOCK_COLA_PEDIDOS);
    colaRespuestas = new Cola<Respuesta>((char*)ARCHIVO_COLAS, C_LOCK_COLA_RESPUESTAS);

    srand (time(NULL) + getpid());
}

void Entrada :: deinicializar() {
    delete colaPedidos;
    delete colaRespuestas;
    inicio.liberar();

    stringstream ss;
    ss << "Entrada " << nombre << "(" << nroEstacionamiento << ") deinicializada";
    Logger::write(INFO, ss.str());
}

void Entrada :: recibirAutos() {

    stringstream info;
    info << "Entrada " << nombre << "(" << nroEstacionamiento << ") inicia simulacion por "
    		<< tiempoSimulacion << " horas";
    Logger::write(INFO, info.str());

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

void Entrada::comunicarCierre() {
	Pedido pedido;
	pedido.mtype = P_TERMINO_ENTRADA;
	pedido.pid = pid;
	pedido.nroEstacionamiento = nroEstacionamiento;

	colaPedidos->escribir(pedido);
}

bool Entrada :: hayLugar() {
	Pedido pedido;
	pedido.mtype = P_OCUPO_LUGAR;
	pedido.pid = pid;
	pedido.nroEstacionamiento = nroEstacionamiento;

	colaPedidos->escribir(pedido);

	Respuesta respuesta;
	colaRespuestas->leer(pid, &respuesta);

	return respuesta.respuesta.habiaLugar;
}

unsigned Entrada :: autosPorHora() {
    return rand() % AUTOS_POR_HORA;
}

void Entrada::esperarAutos() {
	while (wait(NULL) > 0);
}
