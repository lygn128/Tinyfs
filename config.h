//
// Created by lygn128 on 16-3-14.
//

#ifndef TFNODE_CONFIG_H
#define TFNODE_CONFIG_H

#include <map>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;


class config {
    map <string,string> data;
    char * raw;
public:
    int loadConfig(char *configPath);
    config();
    void display();
    int  getValueInt(string key);
    string getValueForString(string key);
//    static int LoadConfig(char *configPath);

};


#endif //TFNODE_CONFIG_H
