#ifndef MENSAJES_H_
	#define MENSAJES_H_

	#include <sys/types.h>

	// Tipos de pedido
	// Auto paga y desocupa posicion
	#define P_PAGO_DESOCUPO		1
	// Auto solicita una posicion vacia
	#define P_PIDO_LUGAR		2
	// Entrada intenta reservar un lugar
	#define P_OCUPO_LUGAR		3
	// Administrador consulta estado de su estacionamiento
	#define P_CONSULTA_ESTADO	4
	// Salida hace que Auto se retira
	#define P_SE_VA				5
	// Salida hace que se libere un lugar especificado
	#define P_LIBERA_LUGAR		6
	// Se utiliza para dar aviso de fin de simulacion
	#define P_FINALIZAR		7
	// Entrada termina su ejecucion
	#define P_TERMINO_ENTRADA			8
	// Administrador termina su ejecucion
	#define P_TERMINO_ADMINISTRADOR		9

	typedef struct Pedido {
		long mtype;
		pid_t pid;
		int nroEstacionamiento;

		// Para cuando el auto paga
		unsigned nroLugar;
		unsigned duracionEstadia;
		// Para cuando el auto sale o el adm
		// indica finalizacion a la salida
		int tipoMensaje;
	} Pedido;

	typedef struct EstadoEstacionamiento {
		int facturacion;
		int lugaresLibres;
	} EstadoEstacionamiento;

	typedef struct Respuesta {
		long mtype;
		union Resultado {
			// Respuesta a ocupacion de lugar (Entrada)
			bool habiaLugar;
			// Respuesta a pedido de lugar (Auto)
			unsigned lugarOtorgado;
			// Respuesta a consulta de estado (Adm)
			EstadoEstacionamiento estado;
			// Repuesta a liberar (Salida)
			bool lugarLiberado;
			// Repuesta a pagar (Auto)
			bool pagoAceptado;
		} respuesta;
	} Respuesta;

#endif /* MENSAJES_H_ */
