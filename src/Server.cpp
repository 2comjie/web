#include "Server.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <functional>

#include "Acceptor.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"

#define BUFFER_READ 1024
Server::Server(EventLoop* _loop) : loop(_loop), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnect, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server() {
    delete acceptor;
}

void Server::handleReadEvent(int sockfd) {
    char buf[BUFFER_READ];
    while (true) {  // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if (bytes_read == -1 && errno == EINTR) {  // 客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if (bytes_read == 0) {  // EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);  // 关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

void Server::newConnect(Socket* servSock) {
    InetAddress* clntAddr = new InetAddress();
    Socket* clntSocket = new Socket(servSock->accept(clntAddr));
    printf("new client fd %d! IP: %s Port: %d\n", clntSocket->getFd(), inet_ntoa(clntAddr->addr.sin_addr), ntohs(clntAddr->addr.sin_port));
    clntSocket->setnonblocking();
    Channel* ch = new Channel(clntSocket->getFd(), loop);
    ch->setEvents(EPOLLIN | EPOLLET);
    ch->setCallback(std::bind(&Server::handleReadEvent, this, clntSocket->getFd()));
    ch->updateChannel();
}