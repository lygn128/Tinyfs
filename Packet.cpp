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

int Packet::readHeader(Connection *connection) {
    int n = connection->ReadBytes((Byte*)&magic,HEADERLEN);
    chunkid = LittleEndian::Uint32((Byte*)&chunkid);
    offset  = LittleEndian::Uint64((Byte*)&offset);
    size    = LittleEndian::Uint64((Byte*)&size);
    parCount= LittleEndian::Uint64((Byte*)&parCount);
    for(int i = 0;i < parCount;i++) {
        parLen[i] = LittleEndian::Uint32((Byte*)parLen +i);
    }
}

int Packet::readBody(Connection *connection) {
    int num = size;
    for(int i = 0;i< parCount;i++){
        num += parLen[i];
    }
    int n = connection->ReadBytes((Byte*)&dataArry,num);
}

int Packet::readPacket(Connection * connection) {
    int headlength = readHeader(connection);
    printf("read header %d\n",headlength);
    int body       = readBody(connection);
    printf("read body %d\n",body);
}