#pragma once

#include <sys/socket.h>

class InetAddress;
class Socket {
   private:
    int fd;

   public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(const InetAddress*);  // 绑定
    void listen();                  // 监听
    int accept(InetAddress*);       // 接收
    void setnonblocking();          // 设置非阻塞
    void setAddrReuse();            // 设置地址复用

    int getFd() const;
};