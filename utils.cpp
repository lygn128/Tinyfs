//
// Created by lygn128 on 16-3-17.
//

#include "utils.h"
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


bool isDirExist(char * path) {
    int fd = open(path,O_RDONLY);
    return fd > 0;
}


void printError(int errnum,int line ) {
    printf("err num %d %d\n",line,errnum);
}