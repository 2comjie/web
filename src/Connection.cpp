#include "Connection.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <functional>

#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#define READ_BUFFER 1024
Connection::Connection(EventLoop* _loop, Socket* _sock) : loop(_loop), sock(_sock), channel(nullptr), inBuffer(new std::string()), readBuffer(nullptr) {
    channel = new Channel(loop, sock->getFd());
    channel->setEvents(EPOLLIN | EPOLLET);
    channel->setCallback(std::bind(&Connection::echo, this, sock->getFd()));
    channel->updateChannel();
    readBuffer = new Buffer();
}

Connection::~Connection() {
    delete channel;
    delete sock;
    delete readBuffer;
    delete inBuffer;
}

void Connection::echo(int sockfd) {
    char buf[READ_BUFFER];
    while (true) {  // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            readBuffer->append(buf, bytes_read);
        } else if (bytes_read == -1 && errno == EINTR) {  // 客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            printf("message from client fd %d: %s\n", sockfd, readBuffer->c_str());
            write(sockfd, readBuffer->c_str(), readBuffer->size());
            readBuffer->clear();
            break;
        } else if (bytes_read == 0) {  // EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            deleteConnectionCallback(sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb) {
    deleteConnectionCallback = _cb;
}