//
// Created by lygn128 on 16-3-17.
//

#ifndef TFNODE_UTILS_H
#define TFNODE_UTILS_H
#define offsetof(type, member) ((size_t) &((type *)0)->member)

#include <cstdio>


bool isDirExist(char * path);

void printError(int errnum,int line );
char *dirCat(char* dir,char * dir2);
int fileOpen(char*fileName);

#endif //TFNODE_UTILS_H
