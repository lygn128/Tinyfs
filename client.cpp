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
#include "utils.h"


int main() {
    Connection * connection = new Connection();
    char * addr =  "127.0.0.1";
    int buffsize = 64 *1024 * 1024;
    int result = connection->TFconnect(addr, 10001);
    for(int i = 0;i < 1000;i++){
        //connection->fd;
        sleep(1);
        Packet *xx = (Packet*)0;
        int n =  (Byte*)&(xx->dataArry) - (Byte*)xx + buffsize;
        printf("n %d   index %d \n",n,i);
        void * temp  = malloc(n);
        Packet * packet = new (temp) Packet;
        packet->magic   = 10;
        packet->opcode  = nodeWrite;
        packet->chunkid = 1;
        packet->offset  = 1;
        packet->size    = buffsize;
        packet->parCount    = 0;
        int x = packet->writePacket(connection);
        printf("send %d   errornu %d\n",x,errno);
        free(temp);
    }



    sleep(20);


}
