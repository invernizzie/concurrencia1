#ifndef ADMINISTRADOR_H_
	#define ADMINISTRADOR_H_

	#include <cstring>
	#include <cstdlib>
	#include <unistd.h>
	#include <time.h>
	#include <exception>

	#include "constantes.h"
	#include "Auto.h"
	#include "logger.h"
	#include "Proceso.h"
	#include "Cola.h"
	#include "Mensajes.h"
	#include "MemoriaCompartida.h"
	#include "Salida.h"

	class Administrador : public Proceso {
		private:
			int nroEstacionamiento;
			int tiempoSimulacion;
			int capacidad;
			int instanteFinal;
			Cola<Pedido>* colaPedidos;
			Cola<Respuesta>* colaRespuestas;

			void inicializar();
			void deinicializar();
			void finalizarSalidas();
			void consultarPeriodicamente();
			unsigned tiempoEntreConsultas();

		public:
			Administrador(int nroEstacionamiento, int tiempoSimulacion, int capacidad);
			virtual void ejecutar();
			virtual ~Administrador() {};
	};

#endif //ADMINISTRADOR_H_
