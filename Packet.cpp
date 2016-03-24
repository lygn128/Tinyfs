//
// Created by lygn128 on 16-3-24.
//

#include "Packet.h"
#include <unistd.h>
#include <stdlib.h>

Packet::Packet() {
    offset = 0;
}

Data* Packet::marshal() {
    int lenght = 15;
    for(int i = 0;i< 4;i++) {
        lenght += parLen[i];
    }
    lenght += size;
    Data * data = (Data*)malloc(sizeof(Data) + lenght);
    data->buff  = (char *)data +  sizeof(Data);
    data->length = lenght;
}
int Packet::writePacket(Connection *connection) {

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