#ifndef SEMAFORO_H_
	#define SEMAFORO_H_

	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <sys/types.h>
	#include <errno.h>

	class Semaforo {

	private:
		int id;
		int valorInicial;

		int inicializar ();

	public:
		Semaforo ( char* nombre,int valorInicial );
		virtual ~Semaforo();

		int wait (); // decrementa
		int signal (); // incrementa
		void eliminar ();
	};

#endif /* SEMAFORO_H_ */
