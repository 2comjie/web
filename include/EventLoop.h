#pragma once

class Channel;
class Epoll;
class EventLoop {
   private:
    Epoll *ep;  // epoll对象
    bool quit;

   public:
    EventLoop();
    ~EventLoop();

    void loop();                    // 事件循环
    void updateChannel(Channel *);  // 往epoll中添加事件
};