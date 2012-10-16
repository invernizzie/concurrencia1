#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

    #define CAPACIDAD_ESTACIONAMIENTO   400
    #define CANT_ENTRADAS               3
    #define CANT_SALIDAS                2
    #define TIEMPO_SIMULACION           8
    #define PRECIO_SUGERIDO             12
    #define ESTADIA_MAXIMA              6
    #define TIEMPO_ENTRE_ARRIBOS        3
    #define TIEMPO_ENTRE_CONSULTAS      4

    #define ARCHIVO_AUXILIAR            "tmp"
    #define ARCHIVO_LOCK_OCUPACION      "ocupacion_lock.tmp"
    #define ARCHIVO_LOCK_FACTURACION    "facturacion_lock.tmp"

    enum EstadoLugar { LIBRE, OCUPADO, PARA_LIBERAR };

#endif // CONSTANTES_H_INCLUDED
