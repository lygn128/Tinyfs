//
// Created by lygn128 on 16-3-24.
//

#include "Packet.h"
#include <unistd.h>
#include <stdlib.h>

Packet::Packet() {
    offset = 0;
}
int Packet::readPacket(Connection * connection) {
    void * xx = this;
    int num =  read(connection->fd,this + readOffset,11);
    if(readOffset >= 11){
        chunkid = ntohl(chunkid);
        offset  = ntohl(offset);
        size    = ntohl(size);

    }
    readOffset += num;

    if(readOffset >= 11) {
        dataArry[0] = (void*)malloc(size);
        num  = read(connection->fd,dataArry[0],size);
        if(num == -1)return num;
        if(readOffset + num == 11 + size){
            ready = true;
        }
    }
    return num;
}