#include "logger.h"


Logger* Logger::m_pInstance = NULL;

Logger* Logger::Instance()
{
   if (!m_pInstance)
      m_pInstance = new Logger;
      modo = MODO_LOG_DEFAULT;
   return m_pInstance;
}

bool Logger::openLogFile(std::string logFile)
{
  return fd.open( logFile.c_str(), std::ofstream::out);
}

void Logger::writeToLogFile(TipoLog modo, std::string mensaje){
    if (modo >= this->modo)
    {
        //fd << "[" << clock() << " : "  << modo << "]" << mensaje << std::endl;
    	std::string msj = "[" << clock() << " : "  << modo << "]" << mensaje;
    	//write((int)fd,msj.c_str(),msj.length());
    	fd.write(msj.c_str(), msj.length());
    }
}


bool Logger::closeLogFile(){
    fd.close();
    return true;
}

