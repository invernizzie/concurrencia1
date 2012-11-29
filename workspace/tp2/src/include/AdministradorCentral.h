#ifndef ADM_CENTRAL_H
#define ADM_CENTRAL_H

#include "Proceso.h"
#include "Cola.h"
#include "Mensajes.h"

class Estacionamiento {
	public:
		Estacionamiento(int capacidad) {};
};

class AdministradorCentral : public Proceso {
	private:
		int cantEstacionamientos;
		int capacidad;
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

	public:
		AdministradorCentral(int cantEstacionamientos, int capacidad);
		virtual ~AdministradorCentral() {};
		void ejecutar();
};

#endif /* ADM_CENTRAL_H */
