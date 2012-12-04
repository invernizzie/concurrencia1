#ifndef ADM_CENTRAL_H
#define ADM_CENTRAL_H

#include "Proceso.h"
#include "Cola.h"
#include "Mensajes.h"
#include "MemoriaCompartida.h"
#include "Estacionamiento.h"
#include "logger.h"

class AdministradorCentral : public Proceso {
	private:
		int cantEstacionamientos;
		int capacidad;
		int valorHora;
		MemoriaCompartida < unsigned > EstacionamientosActivos;

		Estacionamiento** estacionamiento;
        Cola<Pedido>* colaPedidos;
        Cola<Respuesta>* colaRespuestas;

		void inicializar();
		void deinicializar();
		void servirPedidos();
		void liberarPosicion(Pedido& pedido);
		void asignarPosicion(Pedido& pedido);
		void reservarLugar(Pedido& pedido);
		void informarEstado(Pedido& pedido);
		void cobrarEstadia(Pedido& pedido) ;
		void decrementarEntradasActivas(Pedido& pedido);
		void decrementarAdministradoresActivos(Pedido& pedido);
		bool hayEstacionamientosActivos();

	public:
		AdministradorCentral(int cantEstacionamientos, int capacidad, int valorHora);
		virtual ~AdministradorCentral() {};
		void ejecutar();
};

#endif /* ADM_CENTRAL_H */
