#include "Acceptor.h"

#include <stdio.h>

#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"
Acceptor::Acceptor(EventLoop* _loop) : loop(_loop) {
    sock = new Socket();
    InetAddress* addr = new InetAddress("127.0.0.1", 8888);
    sock->setAddrReuse();
    sock->bind(addr);
    sock->listen();

    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    channel->setReadCallback(cb);
    channel->enableRead();
    channel->setUseThreadPool(false);
    delete addr;
}

Acceptor::~Acceptor() {
    delete sock;
    delete channel;
}

void Acceptor::acceptConnection() {
    InetAddress* clntAddr = new InetAddress();
    Socket* clntSock = new Socket(sock->accept(clntAddr));
    clntSock->setnonblocking();
    printf("new client fd %d! IP: %s Port: %d\n", clntSock->getFd(), inet_ntoa(clntAddr->getAddr().sin_addr), ntohs(clntAddr->getAddr().sin_port));
    newConnectionCallback(clntSock);
    delete clntAddr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb) {
    newConnectionCallback = _cb;
}