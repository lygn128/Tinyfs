//
// Created by lygn128 on 16-3-14.
//

#include "server.h"
#include "config.h"
#include "sds.h"
#include "Global.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "Event.h"
#include <errno.h>
#include "utils.h"




int server::handleConnect(Connection * connection) {
    connection->display();
    connection->Close();
    delete(connection);
}
int server::Start() {
    store = new NimbleStore(dataDir.buff);
//    char buff[64 * 1024];
//    for(int j = 0;j < 200;j++){
//        for(int i = 0;i< CHUNKNUM;i++) {
//            store->Write(i,buff,64 * 1024);
//        }
//    }
    listenAndserve();
}


int server::listenAndserve() {
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    int flag = fcntl(sfd,F_GETFL,0);
    int result = fcntl(sfd,F_SETFL,flag | O_NONBLOCK);
    if(sfd < 0) {
        printf("create socket error");
        exit(-1);
    }

    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(10001);
    printf("port is %d",addr.sin_port);


    int on;
    on = 1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&on, sizeof(on));

    int bid = bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
    if(bid < 0) {

        printf("bind err\n");
        printError(errno,__LINE__);
        exit(-1);
    }

    int lister = listen(sfd,10);
    if(lister < 0){
        printError(errno,__LINE__);
        exit(-1);
    }

    int epollfd = epoll_create1(0);
    int add = EventProcess(epollfd,sfd,EPOLL_CTL_ADD,EPOLLET | EPOLLIN);
    if(add < 0) {
        printError(errno,__LINE__);
        exit(-1);
    }

    int subfd = 0;
    struct sockaddr_in remoteaddr;
    socklen_t len;
    struct epoll_event *events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * MAXEVENTS);
    int num = 0,index = 0,aFd;
    while(true) {
        num = epoll_wait(epollfd,events,MAXEVENTS,-1);
        printf("num = %d\n",num);
        for(index = 0;index < num; index++) {
            aFd = events[index].data.fd;
            printf("afd %d sfd %d\n",aFd,sfd);
            if(aFd == sfd) {
               while((subfd = accept(sfd,(struct sockaddr*)&remoteaddr,&len)) > 0){
                   EventProcess(epollfd,subfd,EPOLL_CTL_ADD,EPOLLET|EPOLLIN);
                   Connection * connection = new Connection(subfd,&remoteaddr);
                   handleConnect(connection);
               }
            }else {

            }
        }

       // subfd = accept(sfd,(struct sockaddr*)&remoteaddr,&len);
        //printf("remoate port %d\n",remoteaddr.sin_port);
        //close(subfd);
    }

}


void server::display() {
    printf("server display\n");
    printf("role is %s\n",role.buff);
    printf("ip is %s \n",ip.buff);
    printf("port is %d\n",port);
    printf("zk is %s\n",zk.buff);
    printf("id is %d\n",id);
    printf("datadir  is %s\n",dataDir.buff);
    printf("logdir   is %s\n",logDir.buff);
}
server::server() {

}

int server::loadConfig(config * config1) {
    role = sds(config1->getValueForString("role").c_str());
    ip   = sds(config1->getValueForString("ip").c_str());
    port = config1->getValueInt("port");
    zk   = sds(config1->getValueForString("zk").c_str());
    id   = config1->getValueInt("id");
    dataDir =  sds(config1->getValueForString("datadir").c_str());
    logDir  =  sds(config1->getValueForString("logdir").c_str());
    display();
}


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


