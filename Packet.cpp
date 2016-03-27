//
// Created by lygn128 on 16-3-24.
//

#include "Packet.h"
#include <unistd.h>
#include <stdlib.h>
#include "Global.h"
#include "LittleEndian.h"
#include <cstdio>
#include <errno.h>


Packet::Packet() {
    parCount = 0;
    offset   = 0;
    readOffset = -1;
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
    printf("err num %d  %d\n",errno,n);
    if(n == len) readOffset = 0;
    if(n < 0 && errno > 0 )return -errno;
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
    int num;
    if(readOffset == 0) {
        num = size;
        for(int i = 0;i< parCount;i++){
            num += parLen[i];
        }
        byteNumTosend = size;
        Packet ** xx = (Packet**)(&byteNumTosend);
        printf("realloc\n");
        *xx  = (Packet*)realloc(this,num + sizeof(Packet));
        printf("afteralloc\n");
    }
    int n = 0;
    while((n = connection->ReadBytes((Byte*)&dataArry + readOffset,byteNumTosend - readOffset)) > 0) {
        printf("read body %d\n",n);
        if(n > 0) {
            readOffset += n;
        }
        if(readOffset == byteNumTosend) {
            finished = true;
        }
    }


    if(n < 0 && errno > 0 )return -errno;
    return n;
}

int Packet::readPacket(Connection * connection) {
    int headlength = 0;
    if(readOffset < 0) {
        headlength = readHeader(connection);
        printf("read header %d\n",headlength);

        if(headlength <= 0) {
            return headlength;
        }
        if(headlength == HEADERLEN) {
            readOffset = 0;
        }
    }

    int body  = readBody(connection);
    if(body <= 0) {
        return body;
    }
    printf("read body %d\n",body);
    if(errno == EAGAIN )
    return headlength + body;
    printf("read body %d\n",headlength +body);
}