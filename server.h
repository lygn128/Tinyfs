//
// Created by lygn128 on 16-3-14.
//

#ifndef TFNODE_SERVER_H
#define TFNODE_SERVER_H

#include <signal.h>
#include "config.h"
#include "sds.h"
#include "NimbleStore.h"
#include "Connection.h"

typedef enum state {
   normal,
    recovery,
    disable
}serverstate;


typedef struct serverContext {
    int pid;
    char * path;
    char ** argv;
}Context;

class server {
    sds role;
    sds ip;
    unsigned  int port;
    sds zk;
    int id;
//    long long free;
//    long long capicity;
//    serverstate state;
    sds zkGrupath;
    sds zkNodepath;
    sds dataDir;
    sds logDir;
    NimbleStore * store;
    int   epollpid;
public:
    Context ctx;
private:
    int listenAndserve();
    int handleConnect(Connection * connection);
public:
    int Start();
    server();
//    int spawnnewprocess(char*path,char * argv[]);
    void HandleSignal(int signum,siginfo_t * info,void * ptr);
    void signProcess();

//private:
    int loadConfig(config * config1);
    void display();

};




//{
//"role": "tfnode",
//"ip": "127.0.0.1",
//"port": "20130",
//"sync": "false",
//"id": "1",
//"dc":"1",
//"mode": "repl",
//"mon":"172.19.126.117",
//"clusterid": "777",
//"prof":"50001",
//"zk":"172.19.126.118:2181;172.19.126.146:2181;172.19.126.148:2181",
//"datadir":"/export/jfs/tfnode/data/testx",
//"metadir":"/export/jfs/tfnode/meta/testx",
//"minstoresize": "1"
//}





#endif //TFNODE_SERVER_H
