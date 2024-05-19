#pragma once
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;
// 处理ACCEPT事件，注册READ事件
class Acceptor {
   private:
    EventLoop* loop;
    Socket* sock;
    InetAddress* addr;
    Channel* acceptChannel;

   public:
    Acceptor(EventLoop*);
    ~Acceptor();
    void acceptConnection();
    std::function<void(Socket*)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
};