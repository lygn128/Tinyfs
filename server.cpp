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
#include <execinfo.h>
#include "utils.h"
#include "Packet.h"
#include "signal.h"

#ifndef __USE_GNU
#define __USE_GNU
	#include <ucontext.h>
	#include <sys/ucontext.h>
	#undef __USE_GNU
#else
#include <ucontext.h>
#include <sys/ucontext.h>
#endif


static NimbleStore *globalStore = NULL;


int readHandler(Connection * connection) {
    Packet * packet = new Packet();
    int num = packet->readPacket(connection);
    if(packet->ready){
        switch (packet->opcode) {
            case nodeWrite:{
                //packet->dataArry[0] = (void*)malloc(packet->size);
                globalStore->Write(0,packet->dataArry[0],packet->size);
                break;
            }
        }
    }
    return num;

}

void HandleSignal(int signum,siginfo_t * info,void * ptr) {
    printf("catch a signal");
//    static int iTime;
//    if (iTime++ >= 1){ /* 容错处理：如果访问 ucontext_t 结构体时产生错误会进入该分支 */
//        printf("ReEnter %s is not allowed!\n", __FUNCTION__);
//        abort();
//    }
//
//    void * array[25];
//    int nSize = backtrace(array, sizeof(array)/sizeof(array[0]));
//    int i;
//    for (i=nSize-3; i>2; i--){ /* 头尾几个地址不必输出 */
//        /* 对array修正一下，使地址指向正在执行的代码 */
//        printf("signal[%d] catched when running code at %x\n", signum, (int)array[i] - 1);
//    }
//
//    if (NULL != ptr){
//        ucontext_t* ptrUC = (ucontext_t*)ptr;
//        int *pgregs = (int*)(&(ptrUC->uc_mcontext.gregs));
//        int eip = pgregs[REG_EIP];
//        if (eip != (int)array[i]){ /* 有的处理器会将出错时的 EIP 入栈 */
//            printf("signal[%d] catched when running code at %x\n", signum, (int)array[i] - 1);
//        }
//        printf("signal[%d] catched when running code at %x\n", signum, eip); /* 出错时的指令地址 */
//    }else{
//        printf("signal[%d] catched when running code at unknown address\n", signum);
//    }



    abort();
}

void signProcess() {
    struct sigaction act;
    int sig = SIGSEGV;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = HandleSignal;
    act.sa_flags = SA_SIGINFO;
    if(sigaction(sig,&act,NULL) < 0) {
        perror("sigaction:");
    }
}

int server::handleConnect(Connection * connection) {
    connection->display();
    connection->Close();
    delete(connection);
}
int server::Start() {
    //signProcess();
    store = new NimbleStore(dataDir.buff);
    globalStore = store;
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
    printf("epofd = %d\n",epollfd);
    Connection * sfdConnect = new Connection(sfd,NULL);
    int add = EventProcess(epollfd,sfd,EPOLL_CTL_ADD,EPOLLET|EPOLLIN|EPOLLOUT,sfdConnect);
    if(add < 0) {
        printError(errno,__LINE__);
        exit(-1);
    }

    int subfd = 0;
    struct sockaddr_in remoteaddr;
    socklen_t len;
    struct epoll_event *eventArry = (struct epoll_event*)malloc(sizeof(struct epoll_event) * MAXEVENTS);
    int num = 0,index = 0;
    while(true) {
        bzero(eventArry,sizeof(struct epoll_event) * MAXEVENTS);
        num = epoll_wait(epollfd,eventArry,MAXEVENTS,-1);
        printf("num = %d\n",num);
        for(index = 0;index < num; index++) {
            Connection * context = (Connection*)eventArry[index].data.ptr;
            int aFd   = context->fd;
            int subfd = -1;
            printf("afd %d sfd %d  evets %d\n",aFd,sfd,eventArry[index].events);
            if(aFd == sfd) {
               while((subfd = accept(sfd,(struct sockaddr*)&remoteaddr,&len)) > 0){

                   Connection * connection = new Connection(subfd,&remoteaddr);
                   //printf("172 addr %x\n",connection);
                   EventProcess(epollfd,subfd,EPOLL_CTL_ADD,EPOLLET|EPOLLIN,connection);
                   //handleConnect(connection);
                   connection->readHandler = &readHandler;
               }
            }else {
                Connection * connection = (Connection*)eventArry[index].data.ptr;
                //printf("179 addr %x\n",connection);
                if(eventArry[index].events & EPOLLIN) {
                    if(connection->readHandler != NULL){
                        int x = connection->readHandler(connection);
                        if(x == 0) {
                            EventProcess(epollfd,aFd,EPOLL_CTL_DEL,0,NULL);
                        }
                    }

                }

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


