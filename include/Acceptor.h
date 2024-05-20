#pragma once
#include <functional>
class EventLoop;
class Socket;
class Channel;
class Acceptor {
   private:
    EventLoop* loop;
    Socket* sock;                                        // 用于监听的套接字
    Channel* channel;                                    // 对应的channel
    std::function<void(Socket*)> newConnectionCallback;  // 处理新连接请求的回调函数

   public:
    Acceptor(EventLoop*);
    ~Acceptor();
    void acceptConnection();  // 接收新的连接
    void setNewConnectionCallback(std::function<void(Socket*)>);
};