#pragma once

class InetAddress;
class Socket {
   private:
    int fd;  // 文件描述符

   public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);  // 绑定地址
    void listen();            // 监听
    void setnonblocking();    // 设置非阻塞

    int accept(InetAddress*);  // 接收请求

    int getFd();
};
