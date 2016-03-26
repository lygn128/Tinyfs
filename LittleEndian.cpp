//
// Created by lygn128 on 16-3-24.
//

#include "LittleEndian.h"


void LittleEndian::PutUint16(Byte   *buff, unsigned short v) {
    buff[0] = buff[0]^buff[1];
    buff[1] = buff[0]^buff[1];
    buff[0] = buff[0]^buff[1];

}

void LittleEndian::PutUint32(Byte *b, unsigned v) {
    for(int i = 0;i < 2;i++) {
        b[i] = b[i]^b[3 -i];
        b[3-i] = b[i]^b[3 -i];
        b[i] = b[i]^b[3 - i];
    }
}


void LittleEndian::PutUint64(Byte *b, unsigned long v) {
    b[0] = (Byte)(v);
    b[1] = (Byte)(v>>8);
    b[2] = (Byte)(v>>16);
    b[3] = (Byte)(v>>24);
    b[4] = (Byte)(v>>32);
    b[5] = (Byte)(v>>40);
    b[6] = (Byte)(v>>48);
    b[7] = (Byte)(v>>56);
}

uint16 LittleEndian::Uint16(Byte *b) {
    return uint16(b[1]) | uint16(b[0])<<8;

}

uint32 LittleEndian::Uint32(Byte *b) {
    return uint32(b[3]) | uint32(b[2])<<8 | uint32(b[1])<<16 | uint32(b[0])<<24;
}


uint64 LittleEndian::Uint64(Byte *b) {
    return uint64(b[0]) | uint64(b[1])<<8 | uint64(b[2])<<16 | uint64(b[3])<<24 |
           uint64(b[4])<<32 | uint64(b[5])<<40 | uint64(b[6])<<48 | uint64(b[7])<<56;
}