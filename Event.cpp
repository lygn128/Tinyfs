//
// Created by lygn128 on 16-3-22.
//

#include "Event.h"
#include <sys/epoll.h>
#include <cstdlib>
#include "string.h"



int EventProcess(int epollfd,int fd,int op,int events,void * aPtr){
    struct epoll_event *event = (struct epoll_event *)malloc(sizeof(struct epoll_event));
    bzero(event,sizeof(struct epoll_event));
    event->data.ptr = aPtr;
    event->events  = op;
    return epoll_ctl(epollfd,op,fd,event);
}