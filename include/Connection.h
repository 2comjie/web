#pragma once
#include <functional>
#include <string>

class Buffer;
class Socket;
class EventLoop;
class Channel;
class Connection {
   private:
    EventLoop* loop;
    Socket* sock;                                           // 对应TCP连接的套接字
    Channel* channel;                                       // 对应的channel
    std::function<void(Socket*)> deleteConnectionCallback;  // 断开连接时的回调函数
    std::string* inBuffer;
    Buffer* readBuffer;

   public:
    Connection(EventLoop*, Socket*);
    ~Connection();

    void echo(int);  // 处理读取事件
    void setDeleteConnectionCallback(std::function<void(Socket*)>);
};