#pragma once

#include <functional>
class Socket;
class EventLoop;
class Channel;
class Connection {
   private:
    EventLoop* loop;
    Socket* sock;
    Channel* channel;
    std::function<void(Socket*)> deleteConnectionCallback;

   public:
    Connection(EventLoop*, Socket*);
    ~Connection();

    void echo(int);
    void setDeleteConnectionCallback(std::function<void(Socket*)>);
};