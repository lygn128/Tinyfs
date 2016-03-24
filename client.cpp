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


int main() {
    Connection * connection = new Connection();
    char * addr =  "127.0.0.1";
    int result = connection->TFconnect(addr, 10001);
    printf("connect result = %d err: %s\n",result,strerror(errno));


    sleep(10);
}
