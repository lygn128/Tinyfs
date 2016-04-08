//
// Created by lygn128 on 16-4-5.
//

#include "Logger.h"
#include "../utils.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <cstdio>
#include <errno.h>

char* buff[7] = {(char*)"debug",(char*)"info",(char*)"warn",(char*)"error",(char*)"read",(char*)"write",(char*)"fatal"};
Logger::Logger(char *logpathx) {
    bool  isExist = isDirExist(logpathx);
    if(isExist == false){
        printf("log path is not exist");
        exit(-1);
    }
    logPath = logpathx;

    for(int i = 0;i < 6; i++) {
        char * filePath = dirCat(logpathx,buff[i]);
        file[i] = fopen(filePath,"arb+");
        setvbuf(file[i],NULL,_IONBF,0);
    }
}

void Logger::setLogLevel(int level) {
    this->loggLevel = level;
}

void Logger::Loginfo(char *str, ...) {
    int temlevel = 1;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;
    va_start( argp, str);
    fprintf(file[temlevel],"%s fuction =[%s]%d行",__FILE__,__FUNCTION__,__LINE__);
    fprintf(file[temlevel],str,argp);
    fprintf(file[temlevel],"\n");
}


void Logger::LogDebug(char *str, ...) {
    int temlevel = 0;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;
    va_start( argp, str);
    fprintf(file[temlevel],str,argp);
    fprintf(file[temlevel],"\n");

}

void  Logger::LogWarn(char * str,...) {
    int temlevel = 2;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;
    va_start( argp, str);
    fprintf(file[temlevel],str,argp);
    fprintf(file[temlevel],"\n");
}
void Logger::LogError(char * str,...) {
    int temlevel = 3;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;
    va_start( argp, str);
    fprintf(file[temlevel],str,argp);
    fprintf(file[temlevel],"\n");
}
void Logger::LogFATAL(char * str,...) {
    int temlevel = 6;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;
    va_start( argp, str);
    fprintf(file[temlevel],str,argp);
    fprintf(file[temlevel],"\n");
}
void Logger::LogRead(char * str,...) {
    int temlevel = 4;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;
    va_start( argp, str);
    fprintf(file[temlevel],str,argp);
    fprintf(file[temlevel],"\n");
}
void Logger::LogWrite(char * str,...) {
    int temlevel = 5;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;
    va_start( argp, str);
    fprintf(file[temlevel],str,argp);
    fprintf(file[temlevel],"\n");
}



