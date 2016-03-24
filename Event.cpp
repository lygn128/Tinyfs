//
// Created by lygn128 on 16-3-22.
//

#include "Event.h"
#include <sys/epoll.h>



int EventProcess(int epollfd,int fd,int op,int events){
    struct epoll_event event;
    event.data.fd = fd;
    event.events  = op;
    return epoll_ctl(epollfd,op,fd,&event);
}