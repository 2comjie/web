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

    std::vector<Channel*> poll(int);  // 调用epoll_wait返回发生的事件
    void updateChannle(Channel*);     // 添加、更新监听的事件
};