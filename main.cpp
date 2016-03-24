#include <iostream>
#include "config.h"
#include "server.h"
#include <cstdio>
#include <cstdlib>

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
    configdir = argv[1];
    srv = new server();
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