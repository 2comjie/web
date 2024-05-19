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
    if (events != nullptr)
        delete[] events;
}

#include <stdlib.h>
std::vector<Channel*> Epoll::poll(int timeout) {
    std::vector<Channel*> chs;
    int nfds = 0;
    nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    for (int i = 0; i < nfds; ++i) {
        Channel* ch = (Channel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        chs.push_back(ch);
    }
    return chs;
}

void Epoll::updateChannle(Channel* ch) {
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = ch->getFd();
    ev.data.ptr = ch;
    ev.events = ch->getEvents();

    if (ch->getInEpoll()) {
        epoll_ctl(epfd, EPOLL_CTL_MOD, ch->getFd(), &ev);
    } else {
        epoll_ctl(epfd, EPOLL_CTL_ADD, ch->getFd(), &ev);
        ch->setInEpoll();
    }
}