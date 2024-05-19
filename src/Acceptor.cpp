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
    channel->setCallback(std::bind(&Acceptor::acceptConnection, this));
    channel->setEvents(EPOLLIN);
    channel->updateChannel();
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
    printf("new conn \n");
    newConnectionCallback(clntSock);
    delete clntAddr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb) {
    newConnectionCallback = _cb;
}