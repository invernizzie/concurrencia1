#include "include/logger.h"

#include <time.h>

using namespace std;

// Para imprimir correctamente el modo
std::ostream& operator<<( ostream& ofs, TipoLog modo )
{
   switch (modo)
   {
      case INFO: ofs << "INFO "; break;
      case DEBUG: ofs << "DEBUG"; break;
      case WARNING: ofs << "WARN "; break;
      case ERROR: ofs << "ERROR"; break;
      case FATAL: ofs << "FATAL"; break;
   }
   return ofs;
}

TipoLog Logger :: modo = MODO_LOG_DEFAULT;

Logger& Logger::getInstance() {
    static Logger* instance = new Logger();
    return *instance;
}

void Logger :: initialize(TipoLog mode, string logFile) {
    getInstance().modo = mode;
    getInstance().openLogFile(logFile);
}

void Logger :: write(TipoLog modo, string mensaje) {
    getInstance().writeToLogFile(modo, mensaje);
}

void Logger::openLogFile(string logFile)
{
    fd.open( logFile.c_str(), ofstream::out);
}

void Logger::writeToLogFile(TipoLog modo, string mensaje){
    if (modo >= this->modo)
        fd << "[" << time(NULL) << " : "  << modo << "] " << mensaje << endl;
}


Logger::~Logger(){
    fd.close();
}

