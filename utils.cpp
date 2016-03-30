//
// Created by lygn128 on 16-3-17.
//

#include "utils.h"
#include "sds.h"
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


bool isDirExist(char * path) {
    int fd = open(path,O_RDONLY);
    close(fd);
    return fd > 0;
}


void printError(int errnum,int line ) {
    printf("err num %d %d\n",line,errnum);
}


char *dirCat(char* dir,char * dir2) {
    if(strncmp(dir2,"/",1) == 0 ){
        return dir2;
    }
    sds * dis  = new sds(dir);
    sds * dis2 = new sds(dir2);
    dis2->sdtrim("./");
    printf("%s\n",dis2->buff);
    dis2->sdsAddprefix("/");
    printf("%s\n",dis2->buff);
    dis->sdcat(dis2);
    delete dis2;
    return dis->buff;
}