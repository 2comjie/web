#pragma once
#include <functional>
class Channel;
class Epoll;
class ThreadPool;
class EventLoop {
   private:
    Epoll *ep;  // epoll对象
    ThreadPool *threadPool;
    bool quit;

   public:
    EventLoop();
    ~EventLoop();

    void loop();                    // 事件循环
    void updateChannel(Channel *);  // 往epoll中添加事件
    void addThread(std::function<void()>);
};