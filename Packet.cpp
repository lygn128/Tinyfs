//
// Created by lygn128 on 16-3-24.
//

#include "Packet.h"
#include <unistd.h>
#include <stdlib.h>
#include "Global.h"
#include "LittleEndian.h"
#include <cstdio>


Packet::Packet() {
    parCount = 0;
    offset   = 0;
}


int Packet::writePacket(Connection *connection) {
    this -> Marshal();
    Byte * xxx = (Byte*)this;
    printf("write num %d \n",byteNumTosend);
    return connection->WritBytes((Byte*)&magic, byteNumTosend);
}

void* Packet::Marshal() {
    int num = (Byte*)&dataArry - (Byte*)&magic;
    byteNumTosend = num;
    Byte * temp = (Byte*)this;
    LittleEndian::PutUint16((Byte*)&chunkid, chunkid);
    LittleEndian::PutUint64((Byte*)&offset , offset);
    LittleEndian::PutUint64((Byte*)&size, size);
    LittleEndian::PutUint32((Byte*)&parCount, parCount);
    byteNumTosend += size;
    for(int i = 0;i < parCount; i++ ) {
        byteNumTosend += parLen[i];
    }
}



int Packet::readHeader(Connection *connection) {
    int len = (Byte*)&dataArry - (Byte*)&magic;
    int n = connection->ReadBytes((Byte*)&magic,len);
    if(n == 0) {
        return 0;
    }
    chunkid = LittleEndian::Uint16((Byte*)&chunkid);
    offset  = LittleEndian::Uint64((Byte*)&offset);
    size    = LittleEndian::Uint64((Byte*)&size);
    parCount= LittleEndian::Uint32((Byte*)&parCount);
    for(int i = 0;i < parCount;i++) {
        parLen[i] = LittleEndian::Uint32((Byte*)parLen +i);
    }
    return n;
}

int Packet::readBody(Connection *connection) {
    int num = size;
    for(int i = 0;i< parCount;i++){
        num += parLen[i];
    }
    byteNumTosend = size;
    Packet ** xx = (Packet**)(&byteNumTosend);
    *xx  = (Packet*)realloc(this,num + sizeof(Packet));
    int n = connection->ReadBytes((Byte*)&dataArry,num);
    if(n == num){
        finished = true;
    }
    return n;
}

int Packet::readPacket(Connection * connection) {
    int headlength = readHeader(connection);
    printf("read header %d\n",headlength);

    if(headlength == 0) {
        return 0;
    }
    int body       = readBody(connection);
//    if(headlength <40){
//        return  0;
//    }
    if(body == 0) {
        return 0;
    }
    printf("read body %d\n",body);
    return headlength + body;
    printf("read body %d\n",headlength +body);
}