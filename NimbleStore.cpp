//
// Created by lygn128 on 16-3-17.
//

#include "NimbleStore.h"
#include "utils.h"
#include "Global.h"
#include <cstdio>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>


NimbleStore::NimbleStore(char *path) {
    bool dirExist;
    dirExist = isDirExist(path);
    if(!dirExist) {
        printf("data dir is not exist\n");
        exit(-1);
    }
    for(int i = 0;i < CHUNKNUM;i++){
        char * tempPAth = (char*)malloc(100);
        sprintf(tempPAth,"%s/%d",path,i);
       // printf("%s\n",tempPAth);
        int fd = open(tempPAth,O_CREAT|O_RDWR|O_APPEND,0666);
        fdArry[i] = fd;
       // printf("fd is %d \n",fdArry[i]);
    }
}

size_t NimbleStore::Fwrite(int chunkId, size_t offset, size_t size, void *data, size_t len) {
    int fd = fdArry[chunkId];
    __glibcxx_assert(fd > 0);
    return pwrite(fd,data,len,offset);
}


size_t NimbleStore::Write(int chunkId, void *data, size_t len) {
    static int i = 0;
//    int fd = fdArry[chunkId];
//    __glibcxx_assert(fd > 0);
//    return write(fd,data,len);
    int fd = fdArry[i++];
    i = i%CHUNKNUM;
//    __glibcxx_assert(fd > 0);
    return write(fd,data,len);
}