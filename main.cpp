#include <iostream>
#include "config.h"
#include "server.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include "utils.h"

using namespace std;

static server *srv;

config* loadconfig(char *path);

int main(int argc,char * argv[]) {
    char * configdir;
    configdir = NULL;
    if(argc < 1 ){
        printf("need the config\n");
        exit(-1);
    }
    //char * path = (new sds(get_current_dir_name()))->sdcat(new sds(argv[0]));
    char * path  = dirCat(get_current_dir_name(),argv[0]);
    char * par   = dirCat(get_current_dir_name(),argv[1]);
    printf(" %s  sdfdasf  %s\n",path,par);
    configdir = argv[1];
    srv = new server();
    Context ctx;
    ctx.pid  = getpid();
    ctx.path = path;

    argv[1] = par;
    memcpy(argv[1],par,strlen(par) + 1);
    ctx.argv = argv;
    srv->ctx = ctx;
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