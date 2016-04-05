//
// Created by lygn128 on 16-3-14.
//

#ifndef TFNODE_SDS_H
#define TFNODE_SDS_H


class sds {
    int   len;
    int   free;
public:
    char  *buff;
public:
    sds(const char * str,int initLen);
    sds(const char * str);
    sds(const sds& ads);
    sds();
    static sds* newsds(char * buff,int initLenght);
    static sds ** splitsds(char * string,int srclen,char *sep,int seplen,int * num);
    void display();
    sds sdtrim(const char *cset);
    sds * sdcat(sds *src);
    sds * sdcat(char * str);
    sds * sdsAddprefix(char * str);
    int getLen();
    short sdstos();


};


#endif //TFNODE_SDS_H
