//
// Created by lygn128 on 16-3-23.
//

#ifndef TFNODE_CONNECTION_H
#define TFNODE_CONNECTION_H
#include <netinet/in.h>



class Connection {
    int fd;
    struct sockaddr_in maddr;
public:
    int Close();
    Connection(int fd,struct sockaddr_in * addr);
    void display();
//    int (*conRead)();
//    int (*)
};


#endif //TFNODE_CONNECTION_H
