#pragma once

#include <map>

class EventLoop;
class Connection;
class Acceptor;
class Socket;
class Server {
   private:
    EventLoop* loop;                         // 事件循环
    Acceptor* acceptor;                      // 接收TCP连接
    std::map<int, Connection*> connections;  // 所有TCP连接

   public:
    Server(EventLoop*);
    ~Server();

    void newConnection(Socket*);  // 处理新建连接
    void deleteConnection(int);   // 处理断开连接
};