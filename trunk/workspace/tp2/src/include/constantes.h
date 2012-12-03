#ifndef CONSTANTES_H_INCLUDED

	#define CONSTANTES_H_INCLUDED
	#define CANTIDAD_ESTACIONAMIENTOS	1
    #define CAPACIDAD_ESTACIONAMIENTO   100
    #define CANT_ENTRADAS               3
    #define CANT_SALIDAS                2
    #define TIEMPO_SIMULACION           10
    #define PRECIO_SUGERIDO             12
    #define ESTADIA_MAXIMA              6
    #define AUTOS_POR_HORA              10
    #define TIEMPO_ENTRE_CONSULTAS      3

    #define DIR_AUXILIAR                "tmp"
    #define ARCHIVO_LOG                 "Estacionamiento.log"
    #define MODO_LOG_DEFAULT            DEBUG

    #define ARCHIVO_AUXILIAR            "tmp/tmp"
    #define ARCHIVO_LOCK_OCUPACION      "tmp/ocupacion_lock.tmp"
    #define ARCHIVO_LOCK_FACTURACION    "tmp/facturacion_lock.tmp"
	#define ARCHIVO_LOCK_CANT_PROCESOS	 "tmp/proceso_lock.tmp"
	#define ARCHIVO_COLAS				 "tmp/colas.tmp"
	#define ARCHIVO_LOCK_CANT_ESTAC		 "tmp/cant_estac_lock.tmp"
	#define ARCHIVOS_COLA_SALIDA		 "tmp/cola_salida_"

	#define C_SHM_CANT_PROCESOS			 'c'
    #define C_SHM_CAPACIDAD             'C'
    #define C_SHM_PRECIO                'P'
    #define C_SHM_TIEMPO_INICIO         't'
    #define C_SHM_POS_OCUPADAS          'p'
    #define C_SHM_VALOR_FACTURADO       'F'
    #define C_SHM_LUGAR_ESTACIONAMIENTO 'l'
	#define C_SHM_CANT_ESTACIONAMIENTOS	 'E'

	#define C_LOCK_COLA_PEDIDOS			'p'
	#define C_LOCK_COLA_RESPUESTAS		'r'

    enum EstadoLugar { LIBRE, OCUPADO, PARA_LIBERAR };
    enum TipoLog { INFO, DEBUG, WARNING, ERROR, FATAL };

#endif // CONSTANTES_H_INCLUDED
