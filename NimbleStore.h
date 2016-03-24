//
// Created by lygn128 on 16-3-17.
//

#ifndef TFNODE_NIMBLESTORE_H
#define TFNODE_NIMBLESTORE_H


#include <glob.h>
#include "sds.h"

class NimbleStore {
    char *dir;
    int fdArry[];
public:
    NimbleStore(char * path);
    size_t  Fwrite(int chunkId,size_t offset,size_t size,void *data,size_t len);
    size_t  Write(int chunkId,void *data,size_t len);
};


#endif //TFNODE_NIMBLESTORE_H
