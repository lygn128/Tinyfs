//
// Created by lygn128 on 16-3-14.
//

#include <unistd.h>
#include "config.h"
#include "sds.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <string.h>



#define  CONFIGBUFFLENGTH   (1024 * 1024)


int config::getValueInt(string key) {
    return atoi(data[key].c_str());
}

string config::getValueForString(string key) {
    return this->data[key];
}
void config::display() {
    cout << "display" << endl;
    map<string,string>::iterator in;
//    for(in = data.begin();in!= data.end();++in) {
//       cout <<"key:"<< in->first << "   value:" << in->second << endl;
//    }
}

config::config(){
    this->raw = (char*)malloc(CONFIGBUFFLENGTH * sizeof(char));
}

int config::loadConfig(char *configPath) {
    int fd,readresult;
    fd = open(configPath,O_RDONLY,0644);
    if(fd == -1){
        printf("read config err %s\n",configPath);
        exit(-1);
    }

    readresult = read(fd,this->raw,CONFIGBUFFLENGTH);
    //printf("hava readed congfig num = %d %s\n",readresult,this->raw);

    int num = 0;
    sds ** sdsArry = sds::splitsds(this->raw, strlen(this->raw),"\n",1,&num);
    int num2 = 0;
    for(int i = 0;i < num;i++) {

        num2 = 0;
        sds ** arry = sds::splitsds(sdsArry[i]->buff,strlen(sdsArry[i]->buff),":",1,&num2);
        if(num2 < 2 )continue;

        arry[0]->sdtrim(", \"\t");
        arry[1]->sdtrim(", \"\t");
        char * key   = arry[0]->buff;
        char * value = arry[1]->buff;
        this->data[string(key)] = string(value);
    }


    //this->display();
    return 0;
}

//static int config::LoadConfig(char *configPath) {
//}