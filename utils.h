//
// Created by lygn128 on 16-3-17.
//

#ifndef TFNODE_UTILS_H
#define TFNODE_UTILS_H

#include <cstdio>


bool isDirExist(char * path);

void printError(int errnum,int line );
char *dirCat(char* dir,char * dir2);

#endif //TFNODE_UTILS_H
