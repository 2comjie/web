#pragma once
#include <sys/socket.h>

struct InetAddress;
class Socket {
   private:
    int fd;

   public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(const InetAddress*);  // 绑定文件描述符
    void listen();                  // 监听文件描述符
    int accept(InetAddress*);       // 接收客户端连接请求

    void setnonblocking();                         // 设置非阻塞
    void setSockOpt(int, const void*, socklen_t);  // 设置socket的相关选项

    int getFd();  // 返回文件描述符
};