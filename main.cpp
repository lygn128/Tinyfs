#include <iostream>
#include "config.h"
#include "server.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "commons/Logger.h"

using namespace std;

server *srv = NULL;


Logger * logger = NULL;
int epollfd = 0;
int sfd     = 0;
int loopSwitch = 0;

int fd = 0;
config* loadconfig(char *path);
int mywrit(char * string1) {
    write(fd,string1,strlen(string1) + 1);
}

void mydump() {
    int pid = getpid();
    char buff[100];
    sprintf(buff,"/home/lygn128/ClionProjects/tfnode/%d.txt",pid);
    fd  = open(buff,O_CREAT|O_RDWR|O_APPEND,0644);//    dup2(fd,STDIN_FILENO);
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);

    setvbuf(stdin,NULL,_IONBF,0);
    setvbuf(stdout,NULL,_IONBF,0);
    setvbuf(stderr,NULL,_IONBF,0);

}
int main(int argc,char * argv[]) {

    printf("pid = %d\n",getpid());
    char * configdir;
    configdir = NULL;
    if(argc < 1 ){
        printf("need the config\n");
        exit(-1);
    }
    char * path  = dirCat(get_current_dir_name(),argv[0]);
    char * par   = dirCat(get_current_dir_name(),argv[1]);
    printf("path =  %s  par or argv[1] %s\n",path,par);
    configdir = argv[1];
    srv = new server();
    Context ctx;
    ctx.pid  = getpid();
    ctx.path = path;

    //argv[1] = par;
    char *ars[50];
    for(int i = 0;i < argc;i++) {
        ars[i] = argv[i];
    }
    ars[0] = path;
    ars[1] = par;
    ars[argc] = NULL;
    //memcpy(argv[0],path,strlen(path) + 1);
    //memcpy(argv[1],par,strlen(par) + 1);
    ctx.argv = ars;
    srv->ctx = ctx;
    printf("argv[0] = %s;argv[1] %s\n",srv->ctx.argv[0],srv->ctx.argv[1]);
    config * config1 = loadconfig(configdir);
    srv->loadConfig(config1);

    srv->Start();
    return 0;
}



config* loadconfig(char *path) {
    config *config1 = new(config);
    int loadconf;
    loadconf = config1->loadConfig(path);
    printf("load config %d\n",loadconf);
    if(loadconf!= -1){
        return config1;
    }
    return NULL;
}