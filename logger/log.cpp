#define LOGGER_NO_EXTRAINFO

#include "log.h"
#include <logger/log.h>
#include <time.h>
#include <sys/time.h>
#ifdef __linux__
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <ios>
#include <iostream>

#include "loggerbackend.h"

LoggerBackend * Logger::backend = NULL;

void Logger::init(const char * logpath, bool keepOldLogs, bool color){
    LoggerBackend * backend = new LoggerBackend(logpath,keepOldLogs);
    backend->setColorSupport(true);
    Logger::backend = backend;
}

Logger::Logger(const char *file, int line, int what){
    genTimestamp();

    this->line = line;
    this->level = what;
    this->file = file;
}

Logger::~Logger(){

    backend->append(timestamp, stream.str(), level, file, line);
}


void Logger::genTimestamp(){
    struct timeval tv;
    gettimeofday( &tv, NULL);


    time_t rawtime = tv.tv_sec;
    struct tm *timeinfo = localtime ( &rawtime );

    char datebuf[512];
    sprintf(datebuf,"%02d:%02d:%02d.%3d",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,tv.tv_usec/1000);

    this->timestamp = datebuf;
}

LoggerBackend *Logger::getBackend()
{
    return backend;
}
