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


int main() {
    Connection * connection = new Connection();
    char * addr =  "127.0.0.1";
    int result = connection->TFconnect(addr, 10001);
    Packet * packet = new Packet();
    packet->opcode  = nodeWrite;
    packet->chunkid = 0;
    packet->offset  = 0;
    packet->size    = 100;
    packet->dataArry[0] = malloc(packet->size);
    packet->parCount    = 0;
    packet->writePacket(connection);

    printf("connect result = %d err: %s\n",result,strerror(errno));



    sleep(10);
}
