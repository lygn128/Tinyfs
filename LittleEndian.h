//
// Created by lygn128 on 16-3-24.
//

#ifndef TFNODE_LITTLEENDIAN_H
#define TFNODE_LITTLEENDIAN_H


#include "Types.h"


class LittleEndian {
public:
    static void  PutUint16(Byte * buff, unsigned short v);
    static void  PutUint32(Byte * buff, unsigned  v);
    static void  PutUint64(Byte * buff, unsigned long v);
    static uint16 Uint16(Byte* buff);
    static uint32 Uint32(Byte* buff);
    static uint64 Uint64(Byte* buff);
};


#endif //TFNODE_LITTLEENDIAN_H
