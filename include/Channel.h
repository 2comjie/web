#pragma once

#include <sys/epoll.h>

#include <functional>
class EventLoop;
class Channel {
   private:
    int fd;                          // 对应的文件描述符
    bool inEpoll;                    // 是否已经在Epoll树中
    EventLoop* loop;                 // 事件循环
    std::function<void()> callback;  // 回调函数
    uint32_t events;                 // 关心的事件
    uint32_t revents;                // 发生的事件

   public:
    Channel(int, EventLoop*);
    ~Channel();

    void handleEvent();

    uint32_t getEvents();
    uint32_t getRevents();
    int getFd();
    bool getInEpoll();

    void setEvents(uint32_t);   // 设置关心的事件
    void setRevents(uint32_t);  // 设置已经发生的事件
    void setInEpoll();          // 设置成已经在epoll树中

    void setCallback(std::function<void()>);  // 设置对应事件发生时的回调函数
    void updateChannel();                     // 在epoll树上刷新
};