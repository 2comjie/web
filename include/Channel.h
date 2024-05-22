#pragma once
#include <sys/epoll.h>

#include <functional>
class EventLoop;
class Socket;
class Channel {
   private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    bool useThreadPool;
    std::function<void()> readCallback;   // 读取事件的回调函数
    std::function<void()> writeCallback;  // 写事件的回调函数

   public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableRead();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool _in = true);
    void useET();

    void setReady(uint32_t);
    void setReadCallback(std::function<void()>);
    void setWriteCallback(std::function<void()>);
    void setUseThreadPool(bool use = true);
};