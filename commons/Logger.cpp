//
// Created by lygn128 on 16-4-5.
//

#include "Logger.h"
#include "../utils.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

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
        fd[i] = fileOpen(filePath);
    }
}


void Logger::setLogLevel(int level) {
    this->loggLevel = level;
}

sds* getBuff(char * format,...) {
    va_list argp;                   /* 定义保存函数参数的结构 */
    int argno = 0;                  /* 纪录参数个数 */
    char *para;                     /* 存放取出的字符串参数 */
    int   parInt;
    float parFloat;

    /* argp指向传入的第一个可选参数，    msg是最后一个确定的参数 */
    va_start( argp, format);

    sds *sd = new sds(format);
    while (1)
    {

        para   = va_arg( argp, char *);                 /*    取出当前的参数，类型为char *. */
        parInt = va_arg(argp,int);

        if(para != NULL) {
            sd->sdcat((char*)para);
            argno++;
            continue;
        }else if(para != NULL) {
            char buff[10];
            memset(buff,'\0',10);
            sprintf(buff,"%d",parInt);
            sd->sdcat(buff);
            argno++;
            continue;
            break;

        }else {
            break;
        }
    }
    sd->sdcat("\n");
    va_end( argp );
    return  sd;
}
void Logger::Loginfo(char *str, ...) {
    int temlevel = 1;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;                   /* 定义保存函数参数的结构 */
//    int argno = 0;                  /* 纪录参数个数 */
//    char *para;                     /* 存放取出的字符串参数 */
//
//    /* argp指向传入的第一个可选参数，    msg是最后一个确定的参数 */
    va_start( argp, str);
//
//    sds *sd = new sds(str);
//    while (1)
//    {
//
//        para = va_arg( argp, char *);                 /*    取出当前的参数，类型为char *. */
//        if(para == NULL)break;
//        //printf("Parameter #%d is: %s/n", argno, para);
//        sd->sdcat((char*)para);
//        argno++;
//    }
//    va_end( argp );
//   va_start(argp, str);
    sds * xx = getBuff(str,argp);
    fwrite(xx->buff,1,xx->getLen(),file[temlevel]);
    delete xx;
}


void Logger::LogDebug(char *str, ...) {
    int temlevel = 0;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;                   /* 定义保存函数参数的结构 */
    int argno = 0;                  /* 纪录参数个数 */
    char *para;                     /* 存放取出的字符串参数 */

    /* argp指向传入的第一个可选参数，    msg是最后一个确定的参数 */
    va_start( argp, str);

    sds *sd = new sds(str);
    while (1)
    {
        para = va_arg( argp, char *);                 /*    取出当前的参数，类型为char *. */
        if(para == NULL)break;
        //printf("Parameter #%d is: %s/n", argno, para);
        sd->sdcat((char*)para);
        argno++;
    }
    va_end( argp );
    write(fd[temlevel],sd->buff,sd->getLen());
    delete sd;

}

void  Logger::LogWarn(char * str,...) {
    int temlevel = 2;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;                   /* 定义保存函数参数的结构 */
    int argno = 0;                  /* 纪录参数个数 */
    char *para;                     /* 存放取出的字符串参数 */

    /* argp指向传入的第一个可选参数，    msg是最后一个确定的参数 */
    va_start( argp, str);

    sds *sd = new sds(str);
    while (1)
    {
        para = va_arg( argp, char *);                 /*    取出当前的参数，类型为char *. */
        if(para == NULL)break;
        //printf("Parameter #%d is: %s/n", argno, para);
        sd->sdcat((char*)para);
        argno++;
    }
    va_end( argp );
    write(fd[temlevel],sd->buff,sd->getLen());
    delete sd;
}
void Logger::LogError(char * str,...) {
    int temlevel = 3;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;                   /* 定义保存函数参数的结构 */
    int argno = 0;                  /* 纪录参数个数 */
    char *para;                     /* 存放取出的字符串参数 */

    /* argp指向传入的第一个可选参数，    msg是最后一个确定的参数 */
    va_start( argp, str);

    sds *sd = new sds(str);
    while (1)
    {
        para = va_arg( argp, char *);                 /*    取出当前的参数，类型为char *. */
        if(para == NULL)break;
        //printf("Parameter #%d is: %s/n", argno, para);
        sd->sdcat((char*)para);
        argno++;
    }
    va_end( argp );
    write(fd[temlevel],sd->buff,sd->getLen());
    delete sd;
}
void Logger::LogFATAL(char * str,...) {
    int temlevel = 6;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;                   /* 定义保存函数参数的结构 */
    int argno = 0;                  /* 纪录参数个数 */
    char *para;                     /* 存放取出的字符串参数 */

    /* argp指向传入的第一个可选参数，    msg是最后一个确定的参数 */
    va_start( argp, str);

    sds *sd = new sds(str);
    while (1)
    {
        para = va_arg( argp, char *);                 /*    取出当前的参数，类型为char *. */
        if(para == NULL)break;
        //printf("Parameter #%d is: %s/n", argno, para);
        sd->sdcat((char*)para);
        argno++;
    }
    va_end( argp );
    write(fd[temlevel],sd->buff,sd->getLen());
    printf("counter a fatal err %s",sd->buff);
    exit(-1);
    delete sd;
}
void Logger::LogRead(char * str,...) {
    int temlevel = 4;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;                   /* 定义保存函数参数的结构 */
    int argno = 0;                  /* 纪录参数个数 */
    char *para;                     /* 存放取出的字符串参数 */

    /* argp指向传入的第一个可选参数，    msg是最后一个确定的参数 */
    va_start( argp, str);

    sds *sd = new sds(str);
    while (1)
    {
        para = va_arg( argp, char *);                 /*    取出当前的参数，类型为char *. */
        if(para == NULL)break;
        //printf("Parameter #%d is: %s/n", argno, para);
        sd->sdcat((char*)para);
        argno++;
    }
    va_end( argp );
    write(fd[temlevel],sd->buff,sd->getLen());
    delete sd;
}
void Logger::LogWrite(char * str,...) {
    int temlevel = 5;
    if(temlevel < this->loggLevel)
        return;
    va_list argp;                   /* 定义保存函数参数的结构 */
    int argno = 0;                  /* 纪录参数个数 */
    char *para;                     /* 存放取出的字符串参数 */

    /* argp指向传入的第一个可选参数，    msg是最后一个确定的参数 */
    va_start( argp, str);

    sds *sd = new sds(str);
    while (1)
    {
        para = va_arg( argp, char *);                 /*    取出当前的参数，类型为char *. */
        if(para == NULL)break;
        //printf("Parameter #%d is: %s/n", argno, para);
        sd->sdcat((char*)para);
        argno++;
    }
    va_end( argp );
    write(fd[temlevel],sd->buff,sd->getLen());
    delete sd;
}



