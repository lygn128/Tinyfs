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
#include <signal.h>
#include <sys/wait.h>

#ifndef __USE_GNU
#define __USE_GNU
	#include <ucontext.h>
	#include <sys/ucontext.h>
	#undef __USE_GNU
#else
#include <ucontext.h>
#include <sys/ucontext.h>
#endif

extern int epollfd;
extern int sfd;
extern server *srv;
extern int loopSwitch[2];
static NimbleStore *globalStore = NULL;
static server      *globalSrv      = NULL;
int sta;


void server::Close() {
    close(loopSwitch[0]);
    close(epollfd);
    //exit(0);
}


void server::stopAccept() {
    //EventProcess(epollfd,sfd,EPOLL_CTL_DEL,0,NULL);
    srv->accepton = false;
}

int readHandler(Connection * connection) {
    if(connection->curretnPacket == NULL || connection->curretnPacket->finished == true){
        delete connection->curretnPacket;
        Packet * packet = new Packet();
        connection->curretnPacket = packet;
    }
    Packet *xx = connection->curretnPacket;
    int x = connection->curretnPacket->readPacket(connection);
    printf("has read %d\n",x);
    if(x == 0)
        return 0;
    switch (connection->curretnPacket->opcode) {
        case nodeRead:{
            break;
        }
        case nodeWrite:{
           // globalStore->Write(0,(Byte*)&(xx->dataArry),xx->size);
            break;
        }
    }
    return x;

}


int spawnnewprocess(char*path,char * argv[]) {
    int temppid;
    temppid = fork();
    switch (temppid) {
        case 0:{
            //close(loopSwitch[0]);
            setsid();
            printf("befor exec %s %s %s %s  pid= %d\n",path,argv[0],argv[1],argv[2],getpid());
            char **env = (char**)malloc(sizeof(char*) * 5);
            env[0] = (char*)malloc(100);
            env[1] = (char*)malloc(100);
            env[2] = NULL;
            sprintf(env[0],"sfd=%d",sfd);
            sprintf(env[1],"epollfd=%d",epollfd);

            //char *env[] = {(char*)sfd,(char*)epollfd,NULL};
            srv->Close();
            printf("env = %s %s",env[0],env[1]);
            int xxx = execve(path,argv,env);
            printf("after exec %d  errnu = %d\n",xxx,errno);
            break;
        }
        case 1:{
            sleep(1);
            //close(loopSwitch[1]);
            srv->stopAccept();
            //srv->stopAccept();
           // wait
            //int xx = execve (path,argv,NULL);

            break;
        }
        default:{
            break;
        }

    }
}

void handleSignal(int signum,siginfo_t * info,void * ptr) {
    printf("catch a signal and pid = %d\n",getpid());
    switch (signum) {
        case SIGUSR1: {
            printf("receive sig siguser1\n");
            srv->stopAccept();
            spawnnewprocess(srv->ctx.path, srv->ctx.argv);
            break;
        }
        case SIGUSR2: {
            printf("receive sig siguser2\n");
            srv->Close();
            //spawnnewprocess(ctx.path, ctx.argv);
            break;
        }
        case SIGCHLD:{
            wait((int *)&sta);
            printf("return stats = %d\n",sta);

        }
        default: {
            printf("reveive signal  %d\n", signum);
            break;
        }
    }
}




void server::signProcess() {
    struct sigaction act;
    int sig = SIGSEGV;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGUSR1);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handleSignal;

//    if(sigaction(sig,&act,&act) < 0) {
//        perror("sigaction:");
//    }
    sigaction(SIGUSR1,&act,NULL);
    sigaction(SIGUSR2,&act,NULL);
    sigaction(SIGPIPE,&act,NULL);
    //sigaction(SIGSEGV,&act,NULL);
    sigaction(SIGCHLD,&act,NULL);
}

int server::handleConnect(Connection * connection) {
    connection->display();
    connection->Close();
    delete(connection);
}
int server::Start() {
    globalSrv = this;
    signProcess();
    store     = new NimbleStore(dataDir.buff);
    globalStore = store;
    listenAndserve();
}

int closeProc(){
    return 0;
}
int server::listenAndserve() {
    char * sfdstr     = getenv("sfd");
    char * epollfdstr = getenv("epollfd");
    printf("sfd = %s epollfd = %s\n",sfdstr,epollfdstr);
    //return 0;
    if(sfdstr){
        sfd = atoi(sfdstr);
    }
    if(epollfdstr) {
        //epollfd = atoi(epollfdstr);
    }
    if(sfd == 0) {
        sfd = socket(AF_INET,SOCK_STREAM,0);
        int flag = fcntl(sfd,F_GETFL,0);
        int result = fcntl(sfd,F_SETFL,flag | O_NONBLOCK);
        if(sfd < 0) {
            printf("create socket error\n");
            exit(-1);
            //closeProc();
        }
        struct sockaddr_in addr;
        bzero(&addr,sizeof(addr));
        addr.sin_family      = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port        = htons(10001);
        printf("port is %d\n",addr.sin_port);


        int on;
        on = 1;
        setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&on, sizeof(on));

        int bid = bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
        if(bid < 0) {

            printf("bind err\n");
            printError(errno,__LINE__);
            exit(-1);
            //closeProc();
        }

        int lister = listen(sfd,10);
        if(lister < 0){
            printError(errno,__LINE__);
            exit(-1);
            //closeProc();
        }else {
            printf("listen sucess\n");
        }

    }

    if(epollfd == 0) {
        epollfd = epoll_create1(0);
        printf("epofd = %d\n",epollfd);

    }
    {
        Connection * sfdConnect = new Connection(sfd,NULL);
        int add = EventProcess(epollfd,sfd,EPOLL_CTL_ADD,EPOLLET|EPOLLIN|EPOLLOUT,sfdConnect);
        if(add < 0) {
            printError(errno,__LINE__);
            exit(-1);
            //closeProc();
        }
    }


    {
        int x = pipe(loopSwitch);
        if(x < 0) {
            printf("create pipe err\n");
        }
        //loopSwitch = open("./loopSwitch",O_CREAT|O_RDWR,0644);
        Connection * sfdConnect = new Connection(loopSwitch[0],NULL);
        int add = EventProcess(epollfd,loopSwitch[0],EPOLL_CTL_ADD,EPOLLET|EPOLLIN|EPOLLOUT,sfdConnect);
//        if(add < 0) {
//            printf("add looperr errno  %d\n",errno);
//        }
        if(add < 0) {
            printError(errno,__LINE__);
            exit(-1);
            //closeProc();
        }
    }

    int subfd = 0;
    struct sockaddr_in remoteaddr;
    socklen_t len;
    struct epoll_event *eventArry = (struct epoll_event*)malloc(sizeof(struct epoll_event) * MAXEVENTS);
    int num = 0,index = 0;
    while(true) {
        bzero(eventArry,sizeof(struct epoll_event) * MAXEVENTS);
        struct sigaction act;
        int sig = SIGSEGV;
        sigemptyset(&act.sa_mask);
        sigaddset(&act.sa_mask,SIGUSR1);
        sigaddset(&act.sa_mask,SIGUSR2);
        num = epoll_pwait(epollfd,eventArry,MAXEVENTS,-1,NULL);
        printf("wait success num = %d  err  %d pid = %d\n",num,errno,getpid());
        if(num == -1) {
            continue;
        }
        for(index = 0;index < num; index++) {
            Connection * context = (Connection*)eventArry[index].data.ptr;
            int aFd   = context->fd;
            int subfd = -1;
            if(aFd == sfd) {
                if(accepton) {
                    while((subfd = accept(sfd,(struct sockaddr*)&remoteaddr,&len)) > 0){

                        Connection * connection = new Connection(subfd,&remoteaddr);
                        //printf("172 addr %x\n",connection);
                        EventProcess(epollfd,subfd,EPOLL_CTL_ADD,EPOLLET|EPOLLIN ,connection);
                        //handleConnect(connection);
                        connection->readHandler = &readHandler;
                    }
                }
            }else if( aFd == loopSwitch[0] ||aFd == loopSwitch[1]){
                printf("pipe closed\n");
                return 0;
            } else {
                Connection * connection = (Connection*)eventArry[index].data.ptr;
                //printf("179 addr %x\n",connection);
                if(eventArry[index].events & EPOLLIN) {
                    if(connection->readHandler != NULL){
                        int x = connection->readHandler(connection);
                        if(x == 0) {
                            EventProcess(epollfd,aFd,EPOLL_CTL_DEL,0,NULL);
                            connection->Close();
                            delete(connection);
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
    accepton = true;
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





