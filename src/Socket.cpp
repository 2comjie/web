#include "Socket.h"

#include <fcntl.h>
#include <unistd.h>

#include "InetAddress.h"
Socket::Socket() : fd(-1) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int _fd) : fd(_fd) {
}

Socket::~Socket() {
    if (fd != -1)
        close(fd);
}

void Socket::bind(const InetAddress* addr) {
    ::bind(fd, (sockaddr*)&addr->addr, addr->addrLen);
}

void Socket::listen() {
    ::listen(fd, SOMAXCONN);
}

int Socket::accept(InetAddress* addr) {
    int clnt = ::accept(fd, (sockaddr*)&addr->addr, &addr->addrLen);
    return clnt;
}

void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void Socket::setSockOpt(int optName, const void* opt, socklen_t optLen) {
    setsockopt(fd, SOL_SOCKET, optName, opt, optLen);
}

int Socket::getFd() {
    return fd;
}