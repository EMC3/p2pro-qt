#ifndef LOGGERBACKEND_H
#define LOGGERBACKEND_H

#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <mutex>
//#include "mingw-bug/mingwbug.h"
typedef struct LogEntry{
    LogEntry(std::string timestamp, std::string string, int level, const char *file, int line);

    std::string timestamp;
    std::string string;
    int level;
    const char * file;
    int line;
} LogEntry;

class LogReciever{
public:
    virtual void onNewLog(LogEntry & entry) = 0;
};

class LoggerBackend{
public:
    LoggerBackend(const char * logpath, bool keepOldLogs);
    ~LoggerBackend();
    void append(std::string timestamp, std::string string, int level, const char *file, int line);
    void setBufferSize(int sz);

    const char * getLevelString(int lvl, bool noCol = false);

    std::list<LogEntry> getCurrentEntries();
    void registerReciever(LogReciever * r);
    void removeReciever(LogReciever * r);
    void setColorSupport(bool enable);
    void setStdOutEnabled(bool enable);
    void replaceStdOutStream(std::ostream * stream);
private:
    std::vector<LogReciever *> recievers;

    std::list<LogEntry> entries;
    int bufferSize;
    std::ofstream logfile;
    std::mutex mutex;
    bool colorsEnabled;
    bool haveLogFile;
    bool logToStdOut;
    std::string getColor();

    std::ostream * logStream;
};


#endif // LOGGERBACKEND_H
