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
class Connection;
struct Packet {
public:
    int      byteNumTosend;
    int      readOffset;
    bool     finished;
    Packet   *next;
    uint8    magic;
    u_int8_t opcode;          //1 + 1  + 2 + 8 + 8 + 4 + 16
    uint16    chunkid;
    time_t   timestamp;
    uint64   offset;
    uint64   size;
    int      parCount;
    int      parLen[4];
    void    *dataArry;

    bool     ready;
public:
    int readPacket(Connection * connection);
    int writePacket(Connection * connection);
    int readHeader(Connection *connection);
    int readBody(Connection *connection);
    Packet();
    Data* marshal();
    void * Marshal();
};


#endif //TFNODE_PACKET_H
