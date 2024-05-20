#pragma once
#include <sys/epoll.h>

#include <functional>
class EventLoop;
class Channel {
   private:
    EventLoop* loop;                 // 事件循环
    int fd;                          // 对应的套接字描述符
    std::function<void()> callback;  // 对应套接字触发事件时候的回调函数
    bool inEpoll;
    uint32_t events;   // 关注的事件
    uint32_t revents;  // 发生的事件

   public:
    Channel(EventLoop*, int);
    ~Channel();

    void handleEvent();  // 调用callback，处理触发事件

    void setEvents(uint32_t);
    void setRevents(uint32_t);
    void setInEpoll();
    void setCallback(std::function<void()>);

    uint32_t getEvents() const;
    uint32_t getRevents() const;
    bool getInEpoll() const;
    int getFd() const;

    void updateChannel();
};