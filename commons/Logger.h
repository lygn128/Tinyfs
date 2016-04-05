//
// Created by lygn128 on 16-4-5.
//

#ifndef TFNODE_LOGGER_H
#define TFNODE_LOGGER_H


#include "../sds.h"

enum {
    DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    FATAL = 4,
    READ  = 5,
    WRITE = 6
}Level;

//"[DEBUG]",
//"[INFO.]",
//"[WARN.]",
//"[ERROR]",
//"[FATAL]",
//"[READ.]",
//"[UPDAT]",

class Logger {
    int loggLevel;
    char* logPath;
    int fd[6];
public:
    Logger(char * logpath);
//    Logger()      delete;
    void setLogLevel(int level);
    void Loginfo(char * string,...);
    void LogDebug(char * string,...);
    void LogWarn(char * string,...);
    void LogError(char * string,...);
    void LogFATAL(char * string,...);
    void LogRead(char * string,...);
    void LogWrite(char * string,...);
    static void log(int fd,char * buff,int length);
    static sds* process(char * string,...);

};


#endif //TFNODE_LOGGER_H
