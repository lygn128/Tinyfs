//
// Created by lygn128 on 16-3-14.
//

#include "sds.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>



//sds* sds::newsds(char *buff, int initLenght) {
//
//}

sds::sds(const sds& adx) {
    this->len = adx.len;
    this->free = adx.free;
    this->buff = (char*)malloc(len + free);
    memcpy(this->buff,adx.buff,len);

}
short sds::sdstos() {
    return atoi(buff);
}

sds::sds() {

}


sds* sds::sdcat(char *str) {
    buff =  (char*)realloc(buff,len + strlen(str) + 1);
    memcpy(buff + len,str,strlen(str) + 1);
    len  = len + strlen(str);
    free = 0;
    return this;
}
sds *sds::sdcat(sds *src) {
    if(free < src->len) {
        char * xx  =  (char*)realloc(buff,len + src->len + 1);
        if(xx == NULL)
            return  NULL;
        buff =xx;
    }
    printf("buff %s %d",src->buff,src->len);
    memcpy(buff + len,src->buff,src->len + 1);
    len  = len + src->len;
    free = 0;
    return this;
}


sds* sds::sdsAddprefix(char *str) {
    if(strlen(str) > free) {
        buff = (char*)realloc((void*)buff,len + strlen(str));
    }
    len += strlen(str);
    memmove(buff + strlen(str),buff,len);
    memcpy(buff,str,strlen(str));
    return this;
}


sds sds::sdtrim(const char *cset) {
    char * sp = buff,*start  = buff;
    char * end = buff + len;
    while(sp <= end && strchr(cset,*sp)) sp++;
    while(end > start && strchr(cset,*end)) end--;;
    int len = (sp > end)?0:end - sp + 1;
    if(buff != sp) memmove(buff,sp,len);
    buff[len] = '\0';
    free = free + this->len - len;
    this->len = len;
    return *this;
}


void sds::display() {
   printf("sds display %s   free = %d len =  %d\n",buff,free,len);
}
sds** sds::splitsds(char *src , int srclen, char *sep, int seplen, int *num) {
    int slots = 40,start = 0,i;
    if(srclen < 1 || seplen < 1) {
        return NULL;
    }
    sds ** sdsArry = (sds**)malloc(sizeof(sds*) * slots);
    if(sdsArry == NULL)
        return NULL;
    int sdsnum = 0;
    for(i = 0;i< srclen - seplen;i++) {
        if(seplen == 1 && src[i] == sep[0] || (memcmp(src+i,sep,seplen) == 0)){
           // printf("split %s  %d \n",src + start, i - start);
            sdsArry[sdsnum] = new sds(src + start,i - start);
            i+= seplen;
           // sdsArry[sdsnum]->display();
            sdsnum++;
            start += (i- start);
        }
    }

    sdsArry[sdsnum] = new sds(src + start, srclen - start);
    sdsnum++;
    *num = sdsnum;
    return sdsArry;

}
int sds::getLen() {
    return len;
}

sds::sds(const char *str,const  int initLen) {
    len = initLen;
    free = 0;
    this->buff = (char *)malloc(initLen + 1);
    memcpy(this->buff,str,initLen);
    buff[initLen + 1] = '\0';
}


sds::sds(const char *str) {
    int initLen = strlen(str) + 1;
    len = initLen - 1;
    free = 0;
    this->buff = (char *)malloc(initLen);
    memcpy(this->buff,str,initLen);
}



