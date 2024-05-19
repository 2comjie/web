#pragma once
#include <sys/epoll.h>

#include <vector>
class Channel;
class Epoll {
   private:
    int epfd;
    epoll_event* events;

   public:
    Epoll();
    ~Epoll();

    std::vector<Channel*> poll(int);
    void updateChannel(Channel*);
};