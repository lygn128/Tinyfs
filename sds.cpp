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





//    sds sdstrim(sds s, const char *cset) {
//        struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
//        char *start, *end, *sp, *ep;
//        size_t len;
//
//        sp = start = s;
//        ep = end = s+sdslen(s)-1;
//        while(sp <= end && strchr(cset, *sp)) sp++;
//        while(ep > start && strchr(cset, *ep)) ep--;
//        len = (sp > ep) ? 0 : ((ep-sp)+1);
//        if (sh->buf != sp) memmove(sh->buf, sp, len);
//        sh->buf[len] = '\0';
//        sh->free = sh->free+(sh->len-len);
//        sh->len = len;
//        return s;
//    }

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



/* Split 's' with separator in 'sep'. An array
 * of sds strings is returned. *count will be set
 * by reference to the number of tokens returned.
 *
 * On out of memory, zero length string, zero length
 * separator, NULL is returned.
 *
 * Note that 'sep' is able to split a string using
 * a multi-character separator. For example
 * sdssplit("foo_-_bar","_-_"); will return two
 * elements "foo" and "bar".
 *
 * This version of the function is binary-safe but
 * requires length arguments. sdssplit() is just the
 * same function but for zero-terminated strings.
 */
//sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count) {
//    int elements = 0, slots = 5, start = 0, j;
//    sds *tokens;
//
//    if (seplen < 1 || len < 0) return NULL;
//
//    tokens = zmalloc(sizeof(sds)*slots);
//    if (tokens == NULL) return NULL;
//
//    if (len == 0) {
//        *count = 0;
//        return tokens;
//    }
//    for (j = 0; j < (len-(seplen-1)); j++) {
//        /* make sure there is room for the next element and the final one */
//        if (slots < elements+2) {
//            sds *newtokens;
//
//            slots *= 2;
//            newtokens = zrealloc(tokens,sizeof(sds)*slots);
//            if (newtokens == NULL) goto cleanup;
//            tokens = newtokens;
//        }
//        /* search the separator */
//        if ((seplen == 1 && *(s+j) == sep[0]) || (memcmp(s+j,sep,seplen) == 0)) {
//            tokens[elements] = sdsnewlen(s+start,j-start);
//            if (tokens[elements] == NULL) goto cleanup;
//            elements++;
//            start = j+seplen;
//            j = j+seplen-1; /* skip the separator */
//        }
//    }
//    /* Add the final element. We are sure there is room in the tokens array. */
//    tokens[elements] = sdsnewlen(s+start,len-start);
//    if (tokens[elements] == NULL) goto cleanup;
//    elements++;
//    *count = elements;
//    return tokens;
//
//    cleanup:
//    {
//        int i;
//        for (i = 0; i < elements; i++) sdsfree(tokens[i]);
//        zfree(tokens);
//        *count = 0;
//        return NULL;
//    }
//}

