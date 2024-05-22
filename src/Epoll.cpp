#include "Epoll.h"

#include <string.h>
#include <unistd.h>

#include "Channel.h"
#define MAX_EVENTS 1024
Epoll::Epoll() : epfd(-1), events(nullptr) {
    epfd = epoll_create1(0);
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd != -1)
        close(epfd);
    delete events;
}

std::vector<Channel*> Epoll::poll(int timeout) {
    std::vector<Channel*> chs;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    for (int i = 0; i < nfds; ++i) {
        Channel* ch = (Channel*)events[i].data.ptr;
        ch->setReady(events[i].events);
        chs.push_back(ch);
    }
    return chs;
}

void Epoll::updateChannel(Channel* channel) {
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if (!channel->getInEpoll()) {
        epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        channel->setInEpoll();
    } else {
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    }
}

void Epoll::deleteChannel(Channel* channel) {
    int fd = channel->getFd();
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    channel->setInEpoll(false);
}