#ifndef LOGGER_H
    #define LOGGER_H

    #include <string>
    #include "constantes.h"
    #include <iostream>
    #include <fstream>

    using namespace std;

    class Logger{
    public:
        static Logger& getInstance();
        static void initialize(TipoLog mode, string logFile);
        static void write(TipoLog modo, string mensaje);
        void openLogFile(std::string logFile);
        void writeToLogFile(TipoLog modo, string mensaje);
        virtual ~Logger();

    private:
        Logger(){};
        Logger(Logger const&){};
        Logger& operator=(Logger const&){ return *this; };

        static TipoLog modo;

        std::ofstream fd;
    };

#endif // LOGGER_H
