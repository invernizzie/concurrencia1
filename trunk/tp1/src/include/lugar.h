#ifndef LUGAR_H
    #define LUGAR_H

    #include "constantes.h"

    class lugar
    {
        private:
            estadoLugar estado;

        public:
            lugar();
            virtual ~lugar();
            estadoLugar getEstadoLugar(){return estado;};
            void setEstadoLugar(estadoLugar e){ estado = e;};

    };

#endif // LUGAR_H
