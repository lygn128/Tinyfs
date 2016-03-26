//
// Created by lygn128 on 16-3-24.
//

#include "client.h"
#include "Packet.h"
#include "Connection.h"
#include <cstdlib>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "Global.h"
#include "LittleEndian.h"
#include <new>


int main() {
    Connection * connection = new Connection();
    char * addr =  "127.0.0.1";
    int result = connection->TFconnect(addr, 10001);
    for(int i = 0;i< 10000;i++){
        Packet *xx = (Packet*)0;
        void * temp  = malloc(HEADERLEN + 200 + 64 *1024);
        Packet * packet = new (temp) Packet;
        packet->magic   = 10;
        packet->opcode  = nodeWrite;
        packet->chunkid = 1;
        packet->offset  = 1;
        packet->size    = 64*1024;
        packet->parCount    = 0;
        packet->writePacket(connection);
    }



    sleep(1);

//    uint64  xx = 0x01020304;
//    Byte *oo = (Byte*)&xx;
//    printf(" %c  %c  %c %c %c %c %c %c\n",oo[0],oo[1],oo[2],oo[3],oo[4],oo[5],oo[6],oo[7]);
//
//    LittleEndian::PutUint64(oo,xx);
//    printf(" %c  %c  %c %c %c %c %c %c\n",oo[0],oo[1],oo[2],oo[3],oo[4],oo[5],oo[6],oo[7]);
//    xx = LittleEndian::Uint64(oo);
//    printf(" %c  %c  %c %c %c %c %c %c\n",oo[0],oo[1],oo[2],oo[3],oo[4],oo[5],oo[6],oo[7]);
  //  printf("connect result = %d err: %s\n",result,strerror(errno));



  //  sleep(10);

//     int  a = 0x01020304;
//
//
//    void * bb = malloc(4);
//
//    int * ooo = (int*)bb;
//    int cc = *(int *)bb;
//
//    LittleEndian::PutUint32((Byte*)bb,a);
//    char * xxx = (char*)bb;
//    printf("%0x\n",*ooo);
}
