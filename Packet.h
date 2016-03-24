//
// Created by lygn128 on 16-3-24.
//

#ifndef TFNODE_PACKET_H
#define TFNODE_PACKET_H


#include <sys/types.h>
#include "Connection.h"

class Packet {
public:
    u_int8_t opcode;          //1 + 2 + 4 + 4
    short    chunkid;
    unsigned int  offset;
    unsigned int  size;
    int      parCount;
    int      parLen[4];
    void    *dataArry[5];
    int  readOffset;
    bool ready;
public:
    int readPacket(Connection * connection);
    Packet();
};


#endif //TFNODE_PACKET_H
