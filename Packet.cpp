//
// Created by lygn128 on 16-3-24.
//

#include "Packet.h"
#include <unistd.h>
#include <stdlib.h>
#include "Global.h"
#include "LittleEndian.h"

Packet::Packet() {
    parCount = 0;
    offset   = 0;
}

//Data* Packet::marshal() {
//    int lenght = HEADERLEN;
//    for(int i = 0;i< PARAMAXCOUNT;i++) {
//        lenght += parLen[i];
//    }
//    lenght += size;
//    Data * data = (Data*)malloc(sizeof(Data) + lenght);
//    data->buff  = (char *)data +  sizeof(Data);
//    data->length = lenght;
//    Byte * xx = (Byte*)data->buff;
//    xx[0] = magic;
//    xx[1] = opcode;
//    LittleEndian::PutUint16(xx + 2,chunkid);
//    LittleEndian::PutUint64(xx + 4,offset);
//    LittleEndian::PutUint64(xx + 12,size);
//    LittleEndian::PutUint32(xx + 20,parCount);
//    for(int i = 0;i < parCount;i++) {
//
//    }
//
//
//
//
//}

//uint8    magic;
//u_int8_t opcode;          //1 + 2 + 4 + 4
//short    chunkid;
//unsigned long  offset;
//unsigned long  size;
//int      parCount;
//int      parLen[4];
//void    *dataArry[5];
//int      readOffset;
//bool     ready;
int Packet::writePacket(Connection *connection) {
    void * temp = this;
    this->Marshal();
    return connection->WritBytes(temp,byteNumTosend);
}

void* Packet::Marshal() {
    byteNumTosend = 40;
    Byte * temp = (Byte*)this;
    LittleEndian::PutUint16(temp + 2,chunkid);
    LittleEndian::PutUint64(temp + 2 + 2 ,offset);
    LittleEndian::PutUint64(temp + 2 + 2 + 8,size);
    LittleEndian::PutUint32(temp + 2 + 2 + 8 + 8,parCount);
    byteNumTosend += size;
    for(int i = 0;i< parCount;i++){
        byteNumTosend += parLen[i];
    }
}

//uint8    magic;
//u_int8_t opcode;          //1 + 1  + 2 + 8 + 8 + 4 + 16
//short    chunkid;
//unsigned long  offset;
//unsigned long  size;
//int      parCount;
//int      parLen[4];
//void    *dataArry;
//int      readOffset;
//bool     ready;
int Packet::readPacket(Connection * connection) {
    void * xx = this;
    void * temp = this;

    int n = connection->ReadBytes(xx,HEADERLEN);

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