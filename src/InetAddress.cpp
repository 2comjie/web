#include "InetAddress.h"

#include <string.h>
InetAddress::InetAddress(const char* ip, int port) {
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addrLen = sizeof(addr);
}

InetAddress::InetAddress() {
    bzero(&addr, sizeof(addr));
    addrLen = sizeof(addr);
}

InetAddress::~InetAddress() {
}

void InetAddress::setInetAddr(const sockaddr_in& _addr, socklen_t len) {
    addr = _addr;
    addrLen = len;
}

sockaddr_in InetAddress::getAddr() const {
    return addr;
}

socklen_t InetAddress::getAddrLen() const {
    return addrLen;
}
