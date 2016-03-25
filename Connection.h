//
// Created by lygn128 on 16-3-23.
//

#ifndef TFNODE_CONNECTION_H
#define TFNODE_CONNECTION_H
#include <netinet/in.h>



class Connection {
public:
    int fd;
    struct sockaddr_in maddr;
public:
    int Close();
    int (*readHandler)(Connection * connection);
    int (*writeHandler)();
    Connection(int fd,struct sockaddr_in * addr);
    int WritBytes(void * buff,int length);
    int ReadBytes(void * buff,size_t count);
    void display();
    Connection();
    int TFconnect(char *addr, int port);
//    int (*conRead)();
//    int (*)
};


#endif //TFNODE_CONNECTION_H
