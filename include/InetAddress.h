#pragma once

#include <arpa/inet.h>
class InetAddress {
   private:
    sockaddr_in addr;
    socklen_t addrLen;

   public:
    InetAddress(const char*, int);
    InetAddress();
    ~InetAddress();

    void setInetAddr(const sockaddr_in&, socklen_t);
    sockaddr_in getAddr() const;
    socklen_t getAddrLen() const;
};