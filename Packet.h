//
// Created by lygn128 on 16-3-24.
//

#ifndef TFNODE_PACKET_H
#define TFNODE_PACKET_H


typedef struct xxxx {
    int length;
    char  * buff;
}Data;


#include <sys/types.h>
#include "Connection.h"
#include "Types.h"

class Packet {
public:
    int      byteNumTosend;
    int      readOffset;
    uint8    magic;
    u_int8_t opcode;          //1 + 1  + 2 + 8 + 8 + 4 + 16
    short    chunkid;
    unsigned long  offset;
    unsigned long  size;
    int      parCount;
    int      parLen[4];
    void    *dataArry;

    bool     ready;
public:
    int readPacket(Connection * connection);
    int writePacket(Connection * connection);
    Packet();
    Data* marshal();
    void * Marshal();
};


#endif //TFNODE_PACKET_H
