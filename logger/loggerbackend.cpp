#include "loggerbackend.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <algorithm>
//#include <cbytebuf.h>
#include <sstream>

#define COLOR_RED     "\x1b[31;1m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34;1m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_LIGHTCYAN    "\x1b[36;1m"
#define COLOR_GRAY    "\x1b[37m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_PINK   "\x1b[35;1m"
#define COLOR_RGB(r,g,b) "\x1b[38;2;"#r";"#g";"#b"m"

LoggerBackend::LoggerBackend(const char *logpath, bool keepOldLogs)
{
    logToStdOut = true;

    logStream = &std::cout;

    if(logpath == NULL){
        haveLogFile = false;
        bufferSize = 0;
        this->colorsEnabled = false;
        return;
    }

    const char * logPath = logpath;
    struct stat statbuf;
    if(stat(logPath,&statbuf) == 0){
        if(keepOldLogs){
            auto time = localtime(&statbuf.st_mtime);
            int cnt = 0;
            struct stat tmp;
            std::string newNameS;
            do{
                std::stringstream newName;
                newName << logpath << "-" << (time->tm_year+1900) << "." << (time->tm_mon+1) << "." << time->tm_mday;
                if(cnt != 0){
                    newName << "_"<<cnt;
                }
                newName << ".log.gz";
                newNameS = newName.str();
                cnt++;
            }while (stat(newNameS.c_str(),&tmp) == 0);

            //CByteBuf b;
            //b.load(logpath);
            //b.saveGZ(newNameS.c_str());
        }

        unlink(logPath);
    }
    logfile.open(logPath,std::ios_base::out | std::ios_base::app);
    bufferSize = 0;
    this->colorsEnabled = false;
    haveLogFile = true;

}

LoggerBackend::~LoggerBackend()
{
    logfile.close();
}

void LoggerBackend::append(std::string timestamp, std::string string, int level, const char * file, int line)
{
    std::ostringstream outLine;

    if(colorsEnabled){
        if(file == 0){
            outLine << "[" <<COLOR_GRAY<< timestamp <<COLOR_RESET<< "]"<<getLevelString(level)<<": "<<string;
        }else{
            if(line >= 0){
                outLine << "[" <<COLOR_GRAY<< timestamp <<COLOR_RESET<< "] ["<<COLOR_PINK<<file<<":"<<line<<COLOR_RESET<<"]"<<getLevelString(level)<<": "<<string;
            }else{
                outLine << "[" <<COLOR_GRAY<< timestamp <<COLOR_RESET<< "] ["<<COLOR_PINK<<file<<COLOR_RESET<<"]"<<getLevelString(level)<<": "<<string;
            }
        }
        if(string.back() != '\n'){
            outLine <<COLOR_RESET<<std::endl;
        }
    }else{
        if(file == 0){
            outLine << "[" << timestamp << "]"<<getLevelString(level)<<": "<<string;
        }else{
            if(line >= 0){
                outLine << "[" << timestamp << "] ["<<file<<":"<<line<<"]"<<getLevelString(level)<<": "<<string;
            }else{
                outLine << "[" << timestamp << "] ["<<file<<"]"<<getLevelString(level)<<": "<<string;
            }
        }
        if(string.back() != '\n'){
            outLine << std::endl;
        }
    }

    mutex.lock();


    std::string str = outLine.str();

    if(haveLogFile){
        logfile.write(str.data(),str.size());
        logfile.flush();
    }
    if(logToStdOut){
        logStream->write(str.data(),str.size());
        logStream->flush();
    }


    if(recievers.size() > 0 || bufferSize > 0){

        LogEntry entry(timestamp,  string, level,  file,  line);

        for(LogReciever * recv : recievers){
            recv->onNewLog(entry);
        }
        if(bufferSize > 0){
            entries.push_back(entry);
            if(entries.size() > bufferSize)entries.pop_front();
        }
    }
    mutex.unlock();
}

void LoggerBackend::setBufferSize(int sz)
{
    bufferSize = sz;
}

const char *LoggerBackend::getLevelString(int lvl, bool noCol){
    if(!colorsEnabled || noCol){
        if(lvl == 0)       return( " [INFO]" );
        else if(lvl == 1)  return( " [GOOD]" );
        else if(lvl == 2)  return( " [NOTE]" );
        else if(lvl == 3)  return( " [WARN]" );
        else if(lvl == 4)  return( " [CRIT]" );
        else if(lvl < 0)       return("");
        else               return( " [????]" );
    }

    if(lvl == 0)       return(COLOR_CYAN " [INFO]" COLOR_RESET);
    else if(lvl == 1)  return(COLOR_GREEN " [GOOD]" COLOR_RESET);
    else if(lvl == 2)  return(COLOR_YELLOW " [NOTE]" COLOR_RESET);
    else if(lvl == 3)  return(COLOR_YELLOW " [WARN]" COLOR_RESET);
    else if(lvl == 4)  return(COLOR_RED " [CRIT]" COLOR_RESET);
    else if(lvl < 0)       return("");
    else               return(COLOR_RED " [????]" COLOR_RESET);
}

std::list<LogEntry> LoggerBackend::getCurrentEntries()
{
    mutex.lock();
    std::list<LogEntry> copy;
    copy = entries;
    mutex.unlock();
    return copy;
}

void LoggerBackend::registerReciever(LogReciever *r)
{
    mutex.lock();
    recievers.push_back(r);
    mutex.unlock();
}

void LoggerBackend::removeReciever(LogReciever *r)
{
    mutex.lock();
    recievers.erase(std::remove(recievers.begin(), recievers.end(), r), recievers.end());
    mutex.unlock();
}

void LoggerBackend::setColorSupport(bool enable)
{
    this->colorsEnabled = enable;
}

void LoggerBackend::setStdOutEnabled(bool enable)
{
    this->logToStdOut = enable;
}

void LoggerBackend::replaceStdOutStream(std::ostream * stream)
{
    this->logStream = stream;
}



LogEntry::LogEntry(std::string timestamp, std::string string, int level, const char * file, int line)
{
    this->timestamp  = timestamp ;
    this->string     = string    ;
    this->level      = level     ;
    this->file       = file      ;
    this->line       = line      ;
}
#ifdef __ANDROID_API__
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

#endif
std::string LoggerBackend::getColor(){
    using namespace std;
    int r=(int)(rand()%256);
    int g=(int)(rand()%256);
    int b=(int)(rand()%256);
    return "\x1b[38;2;"+to_string(r)+";"+to_string(g)+";"+to_string(b)+"m";
}
