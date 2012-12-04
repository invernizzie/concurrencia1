#ifndef LOGGER_H
    #define LOGGER_H

    #include <string>
	#include <sstream>
    #include "constantes.h"
    #include <iostream>
    #include <fstream>
    #include <cstdio>
	#include <unistd.h>
	#include <ctime>
    #include "LockFile.h"

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
        Logger(): lock(NULL) {};
        Logger(Logger const&): lock(NULL) {};
        Logger& operator=(Logger const&){ return *this; };
        string fechaFormateada();

        static TipoLog modo;

        std::ofstream fd;
        LockFile* lock;
    };

#endif // LOGGER_H
