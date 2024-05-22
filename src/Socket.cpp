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
    sockaddr_in _addr = addr->getAddr();
    ::bind(fd, (sockaddr*)&_addr, addr->getAddrLen());
}

void Socket::listen() {
    ::listen(fd, SOMAXCONN);
}

int Socket::accept(InetAddress* addr) {
    sockaddr_in clntAddr;
    socklen_t addrLen;
    int clnt = ::accept(fd, (sockaddr*)&clntAddr, &addrLen);
    addr->setInetAddr(clntAddr, addrLen);
    return clnt;
}

void Socket::connect(InetAddress* _addr) {
    sockaddr_in addr = _addr->getAddr();
    ::connect(fd, (sockaddr*)&addr, sizeof(addr));
}

void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void Socket::setAddrReuse() {
    int reuse = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
}

int Socket::getFd() const {
    return fd;
}