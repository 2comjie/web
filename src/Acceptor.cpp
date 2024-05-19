#include "Acceptor.h"

#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
Acceptor::Acceptor(EventLoop* _loop) : loop(_loop) {
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    int reuse = 1;
    sock->setSockOpt(SO_REUSEADDR, &reuse, sizeof(reuse));
    acceptChannel = new Channel(sock->getFd(), loop);
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->setEvents(EPOLLIN);
    acceptChannel->updateChannel();
}

Acceptor::~Acceptor() {
    delete sock;
    delete addr;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    newConnectionCallback(sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb) {
    newConnectionCallback = _cb;
}
