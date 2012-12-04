#ifndef ENTRADA_H_
	#define ENTRADA_H_

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
			bool liberarLugar(unsigned posicion, unsigned espera);

		public:
			Salida(int nroEstacionamiento, int numero);
			virtual void ejecutar();
			virtual ~Salida() {};
	};

#endif //ENTRADA_H_