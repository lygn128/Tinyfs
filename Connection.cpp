//
// Created by lygn128 on 16-3-23.
//

#include <unistd.h>
#include "Connection.h"
#include <cstring>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int Connection::Close() {
    return close(fd);
}


int Connection::WritBytes(void *buff, int length) {
    return write(fd,buff,length);
}

int Connection::ReadBytes(void *buff, size_t count) {
    return read(fd,buff,count);
}

Connection::Connection() {
    curretnPacket = NULL;
}


int Connection::TFconnect(char *addr, int port) {
    fd = -1;
    fd  = socket(AF_INET,SOCK_STREAM,0);
    printf("TFconnect = %d\n",fd);
    maddr.sin_family = AF_INET;
    maddr.sin_port   = htons(port);
    inet_aton(addr,&maddr.sin_addr);
    return connect(fd,(struct sockaddr*)&maddr,sizeof(struct sockaddr));
}


//Connection::Connection(char *addr, int port) {
//
//
//}

Connection::Connection(int xfd, struct sockaddr_in *addr) {
    fd = xfd;
    readHandler = NULL;
    if(addr == NULL)
        return ;
    curretnPacket = NULL;
    memcpy(&maddr,addr,sizeof(struct sockaddr_in));
}



void Connection::display() {
    printf("fd is %d remote addr = %d ,port = %d",fd,0,maddr.sin_port);
}