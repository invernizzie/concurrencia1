#ifndef SALIDA_H_
	#define SALIDA_H_

	#include <string>
	#include <sstream>
	#include <cstdlib>
	#include <unistd.h>
	#include <time.h>
	#include <exception>
	#include <stdlib.h>

	#include "constantes.h"
	#include "Auto.h"
	#include "logger.h"
	#include "Proceso.h"
	#include "Cola.h"
	#include "Mensajes.h"

	using namespace std;

	class Salida : public Proceso {
		private:
			int nroEstacionamiento;
			int numero;
			Cola<Pedido>* colaPedidos;
			Cola<Respuesta>* colaRespuestas;
			Cola<Pedido>* colaPedidoSalida;
			Cola<Respuesta>* colaRespuestaSalida;

			void inicializar();
			void recibirAutos();
			bool liberarLugar(pid_t _auto, unsigned posicion, unsigned espera);

		public:
			Salida(int nroEstacionamiento, int numero);
			virtual void ejecutar();
			virtual ~Salida() {};

			static long mtypePara(int nroEstacionamiento, int nroSalida);
	};

#endif //SALIDA_H_
