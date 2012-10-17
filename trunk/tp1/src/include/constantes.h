#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

    #define CAPACIDAD_ESTACIONAMIENTO   400
    #define CANT_ENTRADAS               3
    #define CANT_SALIDAS                2
    #define TIEMPO_SIMULACION           10
    #define PRECIO_SUGERIDO             12
    #define ESTADIA_MAXIMA              6
    #define AUTOS_POR_HORA              10
    #define TIEMPO_ENTRE_CONSULTAS      4
    #define ARCHIVO_LOG                 "log"

    #define ARCHIVO_AUXILIAR            "tmp"
    #define ARCHIVO_LOCK_OCUPACION      "ocupacion_lock.tmp"
    #define ARCHIVO_LOCK_FACTURACION    "facturacion_lock.tmp"

    #define C_SHM_CAPACIDAD             'C'
    #define C_SHM_PRECIO                'P'
    #define C_SHM_TIEMPO_INICIO         't'
    #define C_SHM_POS_OCUPADAS          'p'
    #define C_SHM_VALOR_FACTURADO       'F'
    #define C_SHM_LUGAR_ESTACIONAMIENTO 'l'

    enum EstadoLugar { LIBRE, OCUPADO, PARA_LIBERAR };

#endif // CONSTANTES_H_INCLUDED
