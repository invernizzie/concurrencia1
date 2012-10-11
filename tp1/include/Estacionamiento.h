#ifndef ESTACIONAMIENTO_H
    #define ESTACIONAMIENTO_H
    #include <array>
    #include <map>

    using namespace std;

    class Estacionamiento
    {
        private:
        int lugaresLibres;
        map <int,lugar> lugares;

        void inicializarMap(int lugares){
            for(int i=0, i<lugares, i++)
                this.lugaresLibres.add(i,LIBRE);
        };

        public:
            Estacionamiento(int lugares, int tiempoSimulacion){
                lugaresLibres = lugares;
            };

            Estacionamiento();
            virtual ~Estacionamiento();
        protected:
    };

#endif // ESTACIONAMIENTO_H
