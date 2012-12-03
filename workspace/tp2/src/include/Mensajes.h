#ifndef MENSAJES_H_
#define MENSAJES_H_

#include <sys/types.h>

// Tipos de pedido
// Auto paga y se retira
#define P_PAGO_Y_LIBERO				1
// Auto solicita una posicion vacia
#define P_PIDO_LUGAR				2
// Entrada intenta reservar un lugar
#define P_OCUPO_LUGAR				3
// Administrador consulta estado de su estacionamiento
#define P_CONSULTA_ESTADO			4
// Entrada termina su ejecucion
#define P_TERMINO_ENTRADA			5
// Administrador termina su ejecucion
#define P_TERMINO_ADMINISTRADOR		6

typedef struct Pedido {
	long mtype;
	pid_t pid;
	int nroEstacionamiento;

	// Solo para que el auto pague
	unsigned nroLugar;
	unsigned duracionEstadia;
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
	} respuesta;
} Respuesta;

#endif /* MENSAJES_H_ */
