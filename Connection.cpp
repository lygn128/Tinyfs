//
// Created by lygn128 on 16-3-23.
//

#include <unistd.h>
#include "Connection.h"
#include <cstring>
#include <stdio.h>


int Connection::Close() {
    return close(fd);
}

Connection::Connection(int xfd, struct sockaddr_in *addr) {
    fd = xfd;
    memcpy(&maddr,addr,sizeof(struct sockaddr_in));
}



void Connection::display() {
    printf("fd is %d remote addr = %d ,port = %d",fd,0,maddr.sin_port);
}