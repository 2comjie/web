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

    std::vector<Channel*> poll(int);  // 调用epoll_wait,返回发生的事件
    void updateChannel(Channel*);     // 往epoll树中注册关注的事件
    void deleteChannel(Channel*);     // 在epoll树中删除注册的事件
};