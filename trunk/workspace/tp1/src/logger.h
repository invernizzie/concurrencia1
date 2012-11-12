#ifndef LOGGER_H
    #define LOGGER_H

    #include <string>
    #include "constantes.h"
    #include <iostream>
    #include <fstream>
    #include <cstdio>

    class Logger{
    public:
       static Logger* Instance();
       bool openLogFile(std::string logFile);
       void writeToLogFile(TipoLog modo, std::string mensaje);
       bool closeLogFile();

    private:
       Logger(){};
       Logger(Logger const&){};
       Logger& operator=(Logger const&){};
       static Logger* m_pInstance;
       static TipoLog modo;
       std::ofstream fd;
    };

#endif // LOGGER_H
